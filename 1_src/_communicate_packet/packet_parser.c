/*!
 * \brief parser register configuration packet
 *
 * packet format:
 *          ____________________________________________________________________________________
 * Bytes:  |     B0     {|    B1    |     B2      | B3 B4 B5 B6 |} ....... | Bn-1     |   Bn    |
 * Value:  |  item num  {| item1 id | item1 field | item1 value |} item2~m |   cumulative sum   |
 * Example:|     2       |   50     |     0       |    -20      |    ...   |       0xXXXX       |
 *          ------------------------------------------------------------------------------------
 * ITEM_NUM: 1~255 number of configure item, ITEM_ID: 1~255, ITEM_FIELD: 1~255, ITEM_VALUE: 4Bytes
 * cumulative sum: cumulative sum of item data by characters
 *
 * \param[in] buf   input data buffer
 * \param[in] len   input data length
 * \return    0 if successful, Error ID otherwise
 */
int config_packet_parser(char *buf, int len)
{
    unsigned char item_num;
    int data_len;
    int i;
    config_item_t item;

    if (!buf)
        return -1;//TODO: add param err print

    if (len < (sizeof(char) + sizeof(config_item_t)))
        return -1;//TODO: add item len err print

    item_num = *(unsigned char *)buf;
    data_len = sizeof(char) + item_num * (sizeof(config_item_t));
    if (data_len != len)
        return -1;//TODO: add packet len err print

    buf++;
    for (i = 0; i < item_num; i++) {
        memcpy(&item, buf, sizeof(config_item_t));
        buf += sizeof(config_item_t);
        config_reg(&item);
    }
}
