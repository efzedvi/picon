/**
 * @author      : faraz
 * @file        : tusb_config
 * @created     : Sunday Dec 26, 2021 18:11:53 EST
 */
#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

#define CFG_TUD_CDC             (1)
#define CFG_TUD_CDC_RX_BUFSIZE  (256)
#define CFG_TUD_CDC_TX_BUFSIZE  (256)

// We use a vendor specific interface but with our own driver
#define CFG_TUD_VENDOR            (0)

#endif /* end of include guard TUSB_CONFIG_H */

