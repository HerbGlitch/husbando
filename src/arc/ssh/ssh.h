#ifndef SSH_H_
#define SSH_H_

#include <libssh/libssh.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ssh type that holds ssh connection along with credentials
*/
typedef struct ARC_Ssh {
    ssh_session session;
} ARC_Ssh;

/**
 * @brief a ssh session function
 *
 * @param data user data that can be used within the callback
*/
typedef void (* ARC_Ssh_SessionFn)(void *data);

/**
 * @brief creates ARC_Ssh type
 *
 * @param ssh ARC_Ssh to create
 */
void ARC_Ssh_Create(ARC_Ssh **ssh);

/**
 * @brief destroyes ARC_Ssh type
 *
 * @param ssh ARC_Ssh to destroy
 */
void ARC_Ssh_Destroy(ARC_Ssh *ssh);

/**
 * @brief runs a callback function within a ssh session
 *
 * @param ssh       ARC_Ssh to create and run function in ssh session
 * @param sessionFN callback to run in a ssh session
 */
void ARC_Ssh_RunInSession(ARC_Ssh *ssh, ARC_Ssh_SessionFn sessionFn);

void ARC_Ssh_ExecStrInNewSession(ARC_Ssh *ssh, char *command);

#ifdef __cplusplus
}
#endif

#endif //!SSH_H_