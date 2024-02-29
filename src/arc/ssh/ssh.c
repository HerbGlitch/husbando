#include "arc/ssh/ssh.h"
#include "arc/std/errno.h"
#include <stdint.h>
#include <stdlib.h>
#include <libssh/libssh.h>
#include <errno.h>
#include <string.h>
 
int verify_knownhost(ssh_session session){
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;
 
    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if(rc < 0){
        return -1;
    }
 
    rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
    ssh_key_free(srv_pubkey);
    if(rc < 0){
        return -1;
    }
 
    state = ssh_session_is_known_server(session);
    switch(state){
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
            break;

        case SSH_KNOWN_HOSTS_CHANGED:
            fprintf(stderr, "Host key for server changed: it is now:\n");
            ssh_print_hexa("Public key hash", hash, hlen);
            fprintf(stderr, "For security reasons, connection will be stopped\n");
            ssh_clean_pubkey_hash(&hash);
            return -1;

        case SSH_KNOWN_HOSTS_OTHER:
            fprintf(stderr, "The host key for this server was not found but an other type of key exists.\n");
            fprintf(stderr, "An attacker might change the default server key to confuse your client into thinking the key does not exist\n");
            ssh_clean_pubkey_hash(&hash);
            return -1;

        case SSH_KNOWN_HOSTS_NOT_FOUND:
            fprintf(stderr, "Could not find known host file.\n");
            fprintf(stderr, "If you accept the host key here, the file will be automatically created.\n");
            /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */
 
        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
            fprintf(stderr, "Public key hash: %s\n", hexa);
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            p = fgets(buf, sizeof(buf), stdin);
            if(p == NULL){
                return -1;
            }
 
            cmp = strncasecmp(buf, "yes", 3);
            if(cmp != 0){
                return -1;
            }
 
            rc = ssh_session_update_known_hosts(session);
            if(rc < 0){
                fprintf(stderr, "Error %s\n", strerror(errno));
                return -1;
            }
            break;

        case SSH_KNOWN_HOSTS_ERROR:
            fprintf(stderr, "Error %s", ssh_get_error(session));
            ssh_clean_pubkey_hash(&hash);
            return -1;
    }
 
    ssh_clean_pubkey_hash(&hash);
    return 0;
}

void ARC_Ssh_Create(ARC_Ssh **ssh){
    *ssh = (ARC_Ssh *)malloc(sizeof(ARC_Ssh));

    (*ssh)->session = ssh_new();
    if((*ssh)->session == NULL){
        arc_errno = ARC_ERRNO_NULL;
        ARC_DEBUG_ERR("ARC_Ssh_Create(ssh), ssh session could not be created\n");
        free(*ssh);
        *ssh = NULL;
        return;
    }

    //TODO: temp replace this with value from gtk or config
    ssh_options_set((*ssh)->session, SSH_OPTIONS_HOST, "10.0.0.157");
    ssh_options_set((*ssh)->session, SSH_OPTIONS_USER, "herbglitch");

    int32_t returnCode = ssh_connect((*ssh)->session);
    if(returnCode != SSH_OK){
        arc_errno = ARC_ERRNO_CONNECTION;
        ARC_DEBUG_LOG(arc_errno, "ARC_Ssh_Create(ssh), ssh failed to connect to localhost: %s\n", ssh_get_error((*ssh)->session));
        ssh_free((*ssh)->session);
        free(*ssh);
        *ssh = NULL;
        return;
    }

    if(verify_knownhost((*ssh)->session) < 0){
        arc_errno = ARC_ERRNO_CONNECTION;
        ARC_DEBUG_ERR("ARC_Ssh_Create(ssh), ssh failed to verify knownhost\n");
        ssh_disconnect((*ssh)->session);
        ssh_free((*ssh)->session);
        free(*ssh);
        *ssh = NULL;
    }

    returnCode = ssh_userauth_password((*ssh)->session, NULL, "p4rk0urLyf3");
    if(returnCode != SSH_AUTH_SUCCESS){
        arc_errno = ARC_ERRNO_CONNECTION;
        ARC_DEBUG_LOG(arc_errno, "ARC_Ssh_Create(ssh), ssh failed to authenticate with password: %s\n", ssh_get_error((*ssh)->session));
        ssh_disconnect((*ssh)->session);
        ssh_free((*ssh)->session);
        free(*ssh);
        *ssh = NULL;
    }
}

void ARC_Ssh_Destroy(ARC_Ssh *ssh){
    if(ssh == NULL){
        return;
    }

    ssh_disconnect(ssh->session);
    ssh_free(ssh->session);
    free(ssh);
}

void ARC_Ssh_RunInSession(ARC_Ssh *ssh, ARC_Ssh_SessionFn sessionFn){
    ssh_channel channel;

    channel = ssh_channel_new(ssh->session);
    if(channel == NULL){
        arc_errno = ARC_ERRNO_NULL;
        ARC_DEBUG_ERR("ARC_Ssh_RunInSession(ssh, sessionFn), ssh failed to create channel\n");
        return;
    }

    int32_t returnCode = ssh_channel_open_session(channel);
    if(returnCode != SSH_OK){
        ssh_channel_free(channel);
        arc_errno = ARC_ERRNO_CONNECTION;
        ARC_DEBUG_LOG(arc_errno, "ARC_Ssh_RunInSession(ssh, sessionFn), ssh failed to open session with return code: %d\n", returnCode);
        return;
    }

    // sessionFn(ssh, );
    returnCode = ssh_channel_request_exec(channel, "export DISPLAY=:0 ; volume --inc");
    printf("return code: %d\n", returnCode);

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

void ARC_Ssh_ExecStrInNewSession(ARC_Ssh *ssh, char *command){
    ssh_channel channel;

    channel = ssh_channel_new(ssh->session);
    if(channel == NULL){
        arc_errno = ARC_ERRNO_NULL;
        ARC_DEBUG_ERR("ARC_Ssh_RunInSession(ssh, sessionFn), ssh failed to create channel\n");
        return;
    }

    int32_t returnCode = ssh_channel_open_session(channel);
    if(returnCode != SSH_OK){
        ssh_channel_free(channel);
        arc_errno = ARC_ERRNO_CONNECTION;
        ARC_DEBUG_LOG(arc_errno, "ARC_Ssh_RunInSession(ssh, sessionFn), ssh failed to open session with return code: %d\n", returnCode);
        return;
    }

    returnCode = ssh_channel_request_exec(channel, command);
    if(returnCode != SSH_OK){
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_LOG(arc_errno, "ARC_Ssh_RunInSession(ssh, sessionFn), ssh failed when executing command with error code: %d\n", returnCode);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}