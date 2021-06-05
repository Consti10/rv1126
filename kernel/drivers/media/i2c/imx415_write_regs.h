//
// Created by consti10 on 05.06.21.
//

#ifndef MEDIASEVER_IMX415_WRITE_REGS_H
#define MEDIASEVER_IMX415_WRITE_REGS_H

#define IMX415_REG_VALUE_08BIT		1
#define IMX415_REG_VALUE_16BIT		2
#define IMX415_REG_VALUE_24BIT		3

// why the heck are these methods written to write "up to 4 registers at a time (4x8bit==32bit) when only byte is written at a time anyways"

/* Write registers up to 4 at a time */
static int imx415_write_reg(struct i2c_client *client, u16 reg,
                            u32 len, u32 val)
{
    u32 buf_i, val_i;
    u8 buf[6];
    u8 *val_p;
    __be32 val_be;

    if (len > 4)
        return -EINVAL;

    buf[0] = reg >> 8;
    buf[1] = reg & 0xff;

    val_be = cpu_to_be32(val);
    val_p = (u8 *)&val_be;
    buf_i = 2;
    val_i = 4 - len;

    while (val_i < 4)
        buf[buf_i++] = val_p[val_i++];

    if (i2c_master_send(client, buf, len + 2) != len + 2)
        return -EIO;

    return 0;
}

static int imx415_write_array(struct i2c_client *client,
                              const struct regval *regs)
{
    u32 i;
    int ret = 0;

    for (i = 0; ret == 0 && regs[i].addr != REG_NULL; i++) {
        ret = imx415_write_reg(client, regs[i].addr,
                               IMX415_REG_VALUE_08BIT, regs[i].val);
    }
    return ret;
}

/* Read registers up to 4 at a time */
static int imx415_read_reg(struct i2c_client *client, u16 reg, unsigned int len,
                           u32 *val)
{
    struct i2c_msg msgs[2];
    u8 *data_be_p;
    __be32 data_be = 0;
    __be16 reg_addr_be = cpu_to_be16(reg);
    int ret;

    if (len > 4 || !len)
        return -EINVAL;

    data_be_p = (u8 *)&data_be;
    /* Write register address */
    msgs[0].addr = client->addr;
    msgs[0].flags = 0;
    msgs[0].len = 2;
    msgs[0].buf = (u8 *)&reg_addr_be;

    /* Read data from register */
    msgs[1].addr = client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = len;
    msgs[1].buf = &data_be_p[4 - len];

    ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
    if (ret != ARRAY_SIZE(msgs))
        return -EIO;

    *val = be32_to_cpu(data_be);

    return 0;
}




#endif //MEDIASEVER_IMX415_WRITE_REGS_H
