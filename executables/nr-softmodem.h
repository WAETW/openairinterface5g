#ifndef NR_SOFTMODEM_H
#define NR_SOFTMODEM_H

#include <executables/nr-softmodem-common.h>

#include "flexran_agent.h"
#include "PHY/defs_gNB.h"

#define DEFAULT_DLF 2680000000


/***************************************************************************************************************************************/
/* command line options definitions, CMDLINE_XXXX_DESC macros are used to initialize paramdef_t arrays which are then used as argument
   when calling config_get or config_getlist functions                                                                                 */

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                            command line parameters common to eNodeB and UE                                                                                */
/*   optname                        helpstr             paramflags                  XXXptr                  defXXXval                      type         numelt               */
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CMDLINE_PARAMS_DESC_GNB {  \
    {"rf-config-file",        CONFIG_HLP_RFCFGF,      0,                strptr:(char **)&rf_config_file,    defstrval:NULL,                TYPE_STRING, sizeof(rf_config_file)},\
    {"ulsch-max-errors",      CONFIG_HLP_ULMAXE,      0,                uptr:&ULSCH_max_consecutive_errors, defuintval:0,                  TYPE_UINT,   0},        \
    {"phy-test",              CONFIG_HLP_PHYTST,      PARAMFLAG_BOOL,   iptr:&phy_test,                     defintval:0,                   TYPE_INT,    0},        \
    {"usim-test",             CONFIG_HLP_USIM,        PARAMFLAG_BOOL,   u8ptr:&usim_test,                   defintval:0,                   TYPE_UINT8,  0},        \
    {"mmapped-dma",           CONFIG_HLP_DMAMAP,      PARAMFLAG_BOOL,   uptr:&mmapped_dma,                  defintval:0,                   TYPE_INT,    0},        \
    {"external-clock",        CONFIG_HLP_EXCCLK,      PARAMFLAG_BOOL,   uptr:&clock_source,                 defintval:0,                   TYPE_INT,    0},        \
    {"wait-for-sync",         NULL,                   PARAMFLAG_BOOL,   iptr:&wait_for_sync,                defintval:0,                   TYPE_INT,    0},        \
    {"single-thread-disable", CONFIG_HLP_NOSNGLT,     PARAMFLAG_BOOL,   iptr:&single_thread_flag,           defintval:1,                   TYPE_INT,    0},        \
    {"A" ,                    CONFIG_HLP_TADV,        0,                uptr:&timing_advance,               defintval:0,                   TYPE_UINT,   0},        \
    {"C" ,                    CONFIG_HLP_DLF,         0,                uptr:&(downlink_frequency[0][0]),   defuintval:DEFAULT_DLF,        TYPE_UINT,   0},        \
    {"a" ,                    CONFIG_HLP_CHOFF,       0,                iptr:&chain_offset,                 defintval:0,                   TYPE_INT,    0},        \
    {"d" ,                    CONFIG_HLP_SOFTS,       PARAMFLAG_BOOL,   uptr:(uint32_t *)&do_forms,         defintval:0,                   TYPE_INT8,   0},        \
    {"E" ,                    CONFIG_HLP_TQFS,        PARAMFLAG_BOOL,   i8ptr:&threequarter_fs,             defintval:0,                   TYPE_INT8,   0},        \
    {"K" ,                    CONFIG_HLP_ITTIL,       PARAMFLAG_NOFREE, strptr:&itti_dump_file,             defstrval:"/tmp/itti.dump",    TYPE_STRING, 0},        \
    {"m" ,                    CONFIG_HLP_DLMCS,       0,                uptr:&target_dl_mcs,                defintval:0,                   TYPE_UINT,   0},        \
    {"t" ,                    CONFIG_HLP_ULMCS,       0,                uptr:&target_ul_mcs,                defintval:0,                   TYPE_UINT,   0},        \
    {"W" ,                    CONFIG_HLP_L2MONW,      0,                strptr:(char **)&in_ip,             defstrval:"127.0.0.1",         TYPE_STRING, sizeof(in_ip)},  \
    {"P" ,                    CONFIG_HLP_L2MONP,      0,                strptr:(char **)&in_path,           defstrval:"/tmp/oai_opt.pcap", TYPE_STRING, sizeof(in_path)},\
    {"q" ,                    CONFIG_HLP_STMON,       PARAMFLAG_BOOL,   iptr:&opp_enabled,                  defintval:0,                   TYPE_INT,    0},        \
    {"S" ,                    CONFIG_HLP_MSLOTS,      PARAMFLAG_BOOL,   u8ptr:&exit_missed_slots,           defintval:1,                   TYPE_UINT8,  0},        \
    {"T" ,                    CONFIG_HLP_TDD,         PARAMFLAG_BOOL,   iptr:&tddflag,                      defintval:0,                   TYPE_INT,    0},        \
    {"numerology" ,           CONFIG_HLP_NUMEROLOGY,  PARAMFLAG_BOOL,   iptr:&numerology,                   defintval:0,                   TYPE_INT,    0},        \
    {"emulate-rf" ,           CONFIG_HLP_EMULATE_RF,  PARAMFLAG_BOOL,   iptr:&emulate_rf,                   defintval:0,                   TYPE_INT,    0},        \
    {"parallel-config",       CONFIG_HLP_PARALLEL_CMD,0,                strptr:(char **)&parallel_config,   defstrval:NULL,                TYPE_STRING, 0},        \
    {"worker-config",         CONFIG_HLP_WORKER_CMD,  0,                strptr:(char **)&worker_config,     defstrval:NULL,                TYPE_STRING, 0},        \
    {"s" ,                    CONFIG_HLP_SNR,         0,                dblptr:&snr_dB,                     defdblval:25,                  TYPE_DOUBLE, 0},        \
    {"nbiot-disable",         CONFIG_HLP_DISABLNBIOT, PARAMFLAG_BOOL,   iptr:&nonbiotflag,                  defintval:0,                   TYPE_INT,    0}         \
  }



extern int T_port;
extern int T_nowait;
extern int T_dont_fork;


#include "threads_t.h"
extern threads_t threads;

// In nr-gnb.c
extern void init_gNB(int single_thread_flag,int wait_for_sync);
extern void stop_gNB(int);
extern void kill_gNB_proc(int inst);

// In nr-ru.c
extern void init_NR_RU(char *);
extern void init_RU_proc(RU_t *ru);
extern void stop_RU(int nb_ru);
extern void kill_NR_RU_proc(int inst);
extern void set_function_spec_param(RU_t *ru);

extern void reset_opp_meas(void);
extern void print_opp_meas(void);

extern void init_fep_thread(PHY_VARS_gNB *);

void init_gNB_afterRU(void);

extern int stop_L1L2(module_id_t gnb_id);
extern int restart_L1L2(module_id_t gnb_id);

#endif
