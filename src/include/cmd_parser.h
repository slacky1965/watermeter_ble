#ifndef SRC_INCLUDE_CMD_PARSER_H_
#define SRC_INCLUDE_CMD_PARSER_H_

enum {
    CMD_SET_HOT_COUNTER      = 0xF1,    /* set hot water        */
    CMD_SET_COLD_COUNTER     = 0xF2,    /* set cold water       */
    CMD_SET_LITERS_PER_PULSE = 0xF3,    /* set liters per pulse */
    CMD_CLEAR_WHIYELIST      = 0xF4,    /* reset whitelist      */
    CMD_RESET                = 0xF5,    /* reset module         */
    CMD_MAIN_NOTIFY          = 0xF6,    /* start notify         */
    CMD_LOG_NOTIFY           = 0xF7,    /* send log string      */
    CMD_SET_BINDKEY          = 0xF8,    /* set secure bind key  */
    CMD_RESET_BINDKEY        = 0xF9     /* reset bind key       */
} cmd_key;

void cmd_parser(void * p);

#endif /* SRC_INCLUDE_CMD_PARSER_H_ */
