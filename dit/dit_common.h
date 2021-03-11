/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS DIT(Direct IP Translator) Driver support
 *
 */

#ifndef __DIT_COMMON_H__
#define __DIT_COMMON_H__

#ifndef DIT_DEBUG
#define DIT_DEBUG
#endif

#ifdef DIT_DEBUG
#define DIT_DEBUG_LOW
#endif

#include "dit.h"

#define DIT_VERSION(x, y, z) \
	((((x) & 0xFF) << 24) | (((y) & 0xFF) << 16) | (((z) & 0xFF) << 8))

#if defined(CONFIG_EXYNOS_DIT_VERSION) && (DIT_VERSION(2, 2, 0) == CONFIG_EXYNOS_DIT_VERSION)
#include "dit_2_2_0.h"
#else
#include "dit_2_1_0.h"
#endif

#define DIT_REG_SW_COMMAND			0x0000

/* total: DIT_REG_CLAT_ADDR_MAX, interval: DIT_REG_CLAT_TX_FILTER_INTERVAL */
#define DIT_REG_CLAT_TX_FILTER			0x2000
/* total: DIT_REG_CLAT_ADDR_MAX, interval: DIT_REG_CLAT_TX_PLAT_PREFIX_INTERVAL */
#define DIT_REG_CLAT_TX_PLAT_PREFIX_0		0x2020
#define DIT_REG_CLAT_TX_PLAT_PREFIX_1		0x2024
#define DIT_REG_CLAT_TX_PLAT_PREFIX_2		0x2028
/* total: DIT_REG_CLAT_ADDR_MAX, interval: DIT_REG_CLAT_TX_CLAT_SRC_INTERVAL */
#define DIT_REG_CLAT_TX_CLAT_SRC_0		0x2080
#define DIT_REG_CLAT_TX_CLAT_SRC_1		0x2084
#define DIT_REG_CLAT_TX_CLAT_SRC_2		0x2088
#define DIT_REG_CLAT_TX_CLAT_SRC_3		0x208C

/* total: DIT_REG_NAT_LOCAL_ADDR_MAX, interval: DIT_REG_NAT_LOCAL_INTERVAL */
#define DIT_REG_NAT_LOCAL_ADDR			0x4100

#define DIT_REG_NAT_ZERO_CHK_OFF		0x4144
#define DIT_REG_NAT_ETHERNET_EN			0x414C

/* total: DIT_REG_NAT_LOCAL_ADDR_MAX, interval: DIT_REG_ETHERNET_MAC_INTERVAL */
#define DIT_REG_NAT_ETHERNET_DST_MAC_ADDR_0	0x6000	/* 32 bit */
#define DIT_REG_NAT_ETHERNET_DST_MAC_ADDR_1	0x6004	/* 16 bit */
#define DIT_REG_NAT_ETHERNET_SRC_MAC_ADDR_0	0x6008	/* 32 bit */
#define DIT_REG_NAT_ETHERNET_SRC_MAC_ADDR_1	0x600C	/* 16 bit */
#define DIT_REG_NAT_ETHERNET_TYPE		0x6010	/* 16 bit */

#define DIT_REG_NAT_TX_PORT_INIT_START		0x6210
#define DIT_REG_NAT_TX_PORT_INIT_DONE		0x6214
#define DIT_REG_NAT_RX_PORT_INIT_START		0x6228
#define DIT_REG_NAT_RX_PORT_INIT_DONE		0x622C

/* total: DIT_REG_NAT_LOCAL_PORT_MAX, interval: DIT_REG_NAT_LOCAL_INTERVAL */
#define DIT_REG_NAT_RX_PORT_TABLE_SLOT		0xC000

/* total numbers and intervals */
#define DIT_REG_NAT_LOCAL_ADDR_MAX		(16)
#define DIT_REG_NAT_LOCAL_PORT_MAX		(2048)
#define DIT_REG_NAT_LOCAL_INTERVAL		(4)
#define DIT_REG_ETHERNET_MAC_INTERVAL		(0x20)
#define DIT_REG_CLAT_ADDR_MAX			(8)
#define DIT_REG_CLAT_TX_FILTER_INTERVAL		(4)
#define DIT_REG_CLAT_TX_PLAT_PREFIX_INTERVAL	(12)
#define DIT_REG_CLAT_TX_CLAT_SRC_INTERVAL	(16)

/* macro for DIT register operation */
#define PADDR_LO(paddr)	(paddr & 0xFFFFFFFF)
#define PADDR_HI(paddr)	((paddr >> 32) & 0xF)

#define WRITE_REG_PADDR_LO(dc, paddr, offset) \
		writel(PADDR_LO(paddr), dc->register_base + offset)
#define WRITE_REG_PADDR_HI(dc, paddr, offset) \
		writel(PADDR_HI(paddr), dc->register_base + offset)
#define WRITE_REG_VALUE(dc, value, offset) \
		writel(value, dc->register_base + offset)
#define READ_REG_VALUE(dc, offset) \
		readl(dc->register_base + offset)
#define WRITE_SHR_VALUE(dc, value) \
		writel(value, dc->sharability_base + dc->sharability_offset)
#define BACKUP_REG_VALUE(dc, dst, offset, size) \
		memcpy_fromio(dst, dc->register_base + offset, size)
#define RESTORE_REG_VALUE(dc, src, offset, size) \
		memcpy_toio(dc->register_base + offset, src, size)

/* macro for DIT function pointer */
#define DIT_INDIRECT_CALL(dc, f, ...)				\
	({							\
		dc->f ? dc->f(__VA_ARGS__) : -EOPNOTSUPP;	\
	})

#define DIT_RX_BURST_16BEAT	(0)

enum dit_int_enable_bits {
	TX_DST0_INT_ENABLE_BIT = 0,
	TX_DST1_INT_ENABLE_BIT,
	TX_DST2_INT_ENABLE_BIT,
	RX_DST0_INT_ENABLE_BIT = 3,
	RX_DST1_INT_ENABLE_BIT,
	RX_DST2_INT_ENABLE_BIT,
	ERR_INT_ENABLE_BIT = 14,
};

#define DIT_INT_ENABLE_MASK \
	(BIT(TX_DST0_INT_ENABLE_BIT) | BIT(TX_DST1_INT_ENABLE_BIT) | \
		BIT(TX_DST2_INT_ENABLE_BIT) | \
		BIT(RX_DST0_INT_ENABLE_BIT) | BIT(RX_DST1_INT_ENABLE_BIT) | \
		BIT(RX_DST2_INT_ENABLE_BIT) | \
		BIT(ERR_INT_ENABLE_BIT))

enum dit_desc_ring {
	DIT_DST_DESC_RING_0,
	DIT_DST_DESC_RING_1,
	DIT_DST_DESC_RING_2,
	DIT_DST_DESC_RING_MAX,
	DIT_SRC_DESC_RING = DIT_DST_DESC_RING_MAX,
	DIT_DESC_RING_MAX
};

enum dit_desc_control_bits {
	DIT_DESC_C_RESERVED,	/* Reserved */
	DIT_DESC_C_END,		/* end packet of LRO */
	DIT_DESC_C_START,	/* first packet of LRO */
	DIT_DESC_C_RINGEND,	/* End of descriptor */
	DIT_DESC_C_INT,		/* Interrupt enabled */
	DIT_DESC_C_CSUM,	/* csum enabled */
	DIT_DESC_C_TAIL,	/* last buffer */
	DIT_DESC_C_HEAD		/* first buffer */
};

#define DIT_SRC_KICK_CONTROL_MASK \
	(BIT(DIT_DESC_C_HEAD) | BIT(DIT_DESC_C_TAIL) | \
		BIT(DIT_DESC_C_INT) | BIT(DIT_DESC_C_RINGEND))

enum dit_desc_status_bits {
	DIT_DESC_S_DONE,	/* DMA done */
	DIT_DESC_S_RESERVED,	/* Reserved */
	DIT_DESC_S_TCPCF,	/* Failed TCP csum */
	DIT_DESC_S_IPCSF,	/* Failed IP csum */
	DIT_DESC_S_IGNR,	/* Ignore csum */
	DIT_DESC_S_TCPC,	/* TCP/UDP csum done: should be 0 if IGNR */
	DIT_DESC_S_IPCS,	/* IP header csum done: should be 0 if IGNR */
	DIT_DESC_S_PFD		/* passed packet filter */
};

#define DIT_CHECKSUM_FAILED_STATUS_MASK \
	(BIT(DIT_DESC_S_TCPCF) | BIT(DIT_DESC_S_IPCSF) | BIT(DIT_DESC_S_IGNR))

enum dit_sw_command_bits {
	DMA_INIT_COMMAND_BIT,
	TX_COMMAND_BIT,
	RX_COMMAND_BIT,
};

enum dit_nat_ethernet_en_bits {
	TX_ETHERNET_EN_BIT,
	RX_ETHERNET_EN_BIT,
};

#define DIT_TX_INT_PENDING_MASK \
	(BIT(TX_DST0_INT_PENDING_BIT) | BIT(TX_DST1_INT_PENDING_BIT) | \
		BIT(TX_DST2_INT_PENDING_BIT) | BIT(ERR_INT_PENDING_BIT))

#define DIT_RX_INT_PENDING_MASK \
	(BIT(RX_DST0_INT_PENDING_BIT) | BIT(RX_DST1_INT_PENDING_BIT) | \
		BIT(RX_DST2_INT_PENDING_BIT) | BIT(ERR_INT_PENDING_BIT))

#define DIT_ALL_INT_PENDING_MASK \
	(DIT_TX_INT_PENDING_MASK | DIT_RX_INT_PENDING_MASK)

/* DIT_STATUS
 * zero means idle
 */
enum dit_status_mask {
	TX_STATUS_MASK = 0x0F,
	RX_STATUS_MASK = 0xF0,
};

enum dit_packet_info_bits {
	DIT_PACKET_INFO_UDP_BIT = 6,
	DIT_PACKET_INFO_TCP_BIT,
	DIT_PACKET_INFO_IPV6_BIT = 10,
	DIT_PACKET_INFO_IPV4_BIT,
};

struct dit_desc_info {
	unsigned int src_wp;
	unsigned int src_rp;
	unsigned int dst_wp[DIT_DST_DESC_RING_MAX];
	unsigned int dst_rp[DIT_DST_DESC_RING_MAX];

	unsigned int src_desc_ring_len;
	struct dit_src_desc *src_desc_ring;
	struct sk_buff **src_skb_buf;
	u32 buf_size;

	phys_addr_t pktproc_pbase;
	u32 pktproc_desc_len;
	u32 *pktproc_fore_ptr;

	unsigned int dst_desc_ring_len;
	struct dit_dst_desc *dst_desc_ring[DIT_DST_DESC_RING_MAX];
	struct sk_buff **dst_skb_buf[DIT_DST_DESC_RING_MAX];
};

struct dit_ctrl_t {
	struct device *dev;
	struct link_device *ld;
	struct net_device *netdev;
	struct napi_struct napi;
	int *irq_pending_bit;
	char const **irq_name;
	int *irq_buf;
	int irq_len;
	int irq_affinity;
	int idle_ip_index;

	void __iomem *register_base;
	void __iomem *sharability_base;
	u32 sharability_offset;
	u32 sharability_value;

	u32 hw_version;
	u32 reg_version;
	u32 hw_capabilities;
	bool use_dir[DIT_DIR_MAX];
	bool stop_enqueue[DIT_DIR_MAX];
	bool use_clat;
	bool hal_support;
	bool hal_enqueue_rx;
	u32 rx_extra_desc_ring_len;

	struct dit_desc_info desc_info[DIT_DIR_MAX];

	/* for kicked flag, reg_value_q and init_done */
	spinlock_t src_lock;
	bool kicked[DIT_DIR_MAX];
	bool kick_reserved[DIT_DIR_MAX];
	struct list_head reg_value_q;
	bool init_done;
	bool init_reserved;

	atomic_t init_running;
	atomic_t stop_napi_poll;

#if defined(DIT_DEBUG_LOW)
	int pktgen_ch;
	int force_bypass;
#endif

	bool clat_hal_ready;

	/* every functions should return int for DIT_INDIRECT_CALL */
	int (*get_reg_version)(u32 *version);
	int (*set_reg_upstream)(struct net_device *netdev);
	int (*do_init_hw)(void);
	int (*do_suspend)(void);
	int (*do_resume)(void);
};

struct dit_snapshot_t {
	char *name;
	int head;
	int tail;

	u64 packets;
	/* cumulative amount */
	u64 total_packets;
	u64 clat_packets;

	u32 max_usage;
	u32 alloc_skbs;
};

struct dit_reg_value_item {
	struct list_head list;
	u32 value;
	u32 offset;
};

struct dit_iface {
	u8 upstream_ch;
};

enum dit_dump_bits {
	DIT_DUMP_SNAPSHOT_BIT,
	DIT_DUMP_DESC_BIT,
	DIT_DUMP_PORT_TABLE_BIT,
	DIT_DUMP_MAX,
};

#define DIT_DUMP_ALL \
	(BIT(DIT_DUMP_SNAPSHOT_BIT) | BIT(DIT_DUMP_DESC_BIT) | \
		BIT(DIT_DUMP_PORT_TABLE_BIT))

enum dit_idle_ip {
	DIT_IDLE_IP_ACTIVE = 0,
	DIT_IDLE_IP_IDLE,
};

/*
 * if there is 1 src desc and it is at the ring_end,
 * DIT will reads 3 descs from the ring_end.
 * for the safety, add additional 2 descs.
 */
#define DIT_SRC_DESC_RING_LEN_PADDING	(2)

bool dit_is_kicked_any(void);
int dit_check_dst_ready(enum dit_direction dir, enum dit_desc_ring ring_num);
int dit_enqueue_reg_value_with_ext_lock(u32 value, u32 offset);
int dit_enqueue_reg_value(u32 value, u32 offset);
int dit_read_rx_dst_poll(struct napi_struct *napi, int budget);
int dit_manage_rx_dst_data_buffers(bool fill);
bool dit_is_busy(enum dit_direction dir);
int dit_stop_napi_poll(void);

static inline int dit_dummy(void) { return 0; }
int dit_ver_create(struct dit_ctrl_t *dc_ptr);

#endif /* __DIT_COMMON_H__ */

