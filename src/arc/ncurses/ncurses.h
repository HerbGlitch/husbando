#ifndef SSH_H_
#define SSH_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
*/
typedef struct ARC_NCurses ARC_NCurses;

/**
 * @brief creates ARC_NCurses type
 *
 * @param ncurses ARC_NCurses to create
*/
void ARC_NCurses_Create(ARC_NCurses **ncurses);

/**
 * @brief destroyes ARC_NCurses type
 *
 * @param ncurses ARC_NCurses to destroy
*/
void ARC_NCurses_Destroy(ARC_NCurses *ncurses);

#ifdef __cplusplus
}
#endif

#endif //!SSH_H_
