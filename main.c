// C code that calculate subnetting , for Lan's mahcines. //
// Academic purpose //

////////INCLUDES//////////////////////////////////////
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////
////////MACROS////////////////////////////////////////
#define I8                      __int8_t
#define I16                     __int16_t
#define I32                     __int32_t
#define I64                     __int64_t

#define OK                      (0)
#define NOK                     (1)
#define EXCEPTION               (2)

#define echo(...) printf(__VA_ARGS__) ;   printf("\n")
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SIZE(list) sizeof(list)/sizeof(list[0])
#define ADDRESS_MAX_LEN                     16

#define MAX_ELEMENT(size, n) ({\
        I16 result = 0; \
        for (I8 i = 0 ; i < size; i++) { \
            if (MAX(n, 2 << i) == 2 << i){\
                result = 2 << i;\
                break;\
            }\
        }\
        result;  })

////////////////////////////////////////////////////
//////////////////GLOBALS///////////////////////////
I8 Network_address[ADDRESS_MAX_LEN]      = {0}; 
I8 Broadcast_address[ADDRESS_MAX_LEN]    = {0};
I8 Mask_address[ADDRESS_MAX_LEN]         = {0};


////////////////////////////////////////////////////
/////////FUNCTIONS////////////////////////////////////
I8 from_string_to_integer(I16 *paddress, I8 *Base_address)
{
    char *ch = NULL;
    I8 i = 0;
    I16 dec = 0;

    if (NULL == paddress || NULL == Base_address)
    {
        return NOK;
    }
    ch = strtok(Base_address, ".");
    while (ch != NULL) 
    {
        dec = atoi(ch);
        paddress[i] = dec;
        i++;
        ch = strtok(NULL, ".");
    }
    return OK;
}

I8 Calc_HOSTID(I16 machine_nb)
{
    I16 avlb_ = 0;
    I8 host_id = 0;

    //Assuming that the max number of the machines is 512
    avlb_ = MAX_ELEMENT(10, (machine_nb + 2)); // machines number + (network address + broadcast address)
    host_id = log(avlb_)/log(2);
    return host_id;
}

I8 Calc_Prefix(I8 num_bits)
{
    return (32 - num_bits);
}

I8 Calc_MagicNumber(I8 prefix, I8 octet_num, I8 max_bits, I16 *mask)
{
    I8  nbytes = 0 ;
    I16 sub = 0;
    I8 index = 0;
    I16 ret = 0;

    if(prefix > max_bits)
    {
        nbytes = prefix - max_bits;
    }
    else
    {
        nbytes = max_bits - prefix;
    }
 
    sub = 2 << (nbytes - 1);
    if (sub < 255) {ret = 256 - sub;} else {ret = 0;};
    mask[octet_num - 1] = ret;
    for (index = octet_num ; index < octet_num + (4 - octet_num) ; index++)
    {
        mask[index] = 0;
    }
    return OK;
}

I8 Calc_mask(I8 prefix)
{
    I16 temp_mask[4] = {255, 255, 255, 255};
    temp_mask[3] = 0;
    
    if (0 <= prefix && prefix <= 8)
    {
        Calc_MagicNumber(prefix, 1, 8, temp_mask);
    }
    else if (8 < prefix && prefix <= 16)
    {
        Calc_MagicNumber(prefix, 2, 16, temp_mask);
    }
    else if (16 < prefix && prefix <= 24)
    {
        Calc_MagicNumber(prefix, 3, 24, temp_mask);
    }
    else if (24 < prefix && prefix <= 32)
    {
        Calc_MagicNumber(prefix, 4, 32, temp_mask);
    }
    else
    {
        return NOK;
    }

    sprintf(Mask_address, "%d.%d.%d.%d", temp_mask[0], temp_mask[1], temp_mask[2], temp_mask[3]);
    return OK;
}

I8 Calc_NetworkAddress(void)
{
    I16 i_address[4]  = {0};

    if(!strcmp("", Broadcast_address))
    {
        return EXCEPTION;
    }
    from_string_to_integer(i_address, Broadcast_address);

    if (i_address[3] + 1 < 255)
    {
        i_address[3]++; // moving to next LAN , so we need to get the next address after previous LAN broadcast address
    }
    //PS: For now , No more else if here i Don't think we will pass the range of the third and fourth octets : 192.168.x.x
    else
    {
        i_address[2]++;
    }
    
    memset(Network_address, 0, sizeof(Network_address));
    sprintf(Network_address, "%d.%d.%d.%d", i_address[0], i_address[1], i_address[2], i_address[3]);
    return OK;
}

I8 Calc_BroadcastAddress(I8 host_id)
{
    I16 hosts = 1;
    I16 i_address[4]  = {0};

    if(!strcmp("", Network_address))
    {
        return NOK;
    }
    // 2(0) = 1 already counted 
    for (I8 i = 0 ; i < host_id - 1; i ++)
    {
        hosts+= 2 << i;
    }
    from_string_to_integer(i_address, Network_address);

    // moving to next LAN , so we need to get the next address after previous LAN broadcast address
    if (hosts > 65025)
    {
        I16 index = hosts / 255;
        i_address[3] = 255;
        i_address[2] = 255;
        i_address[1] += index - 1; 
    }

    else if (hosts > 255 && hosts <= 65025)
    {
        I16 index = hosts / 255;
        i_address[3] = 255;
        i_address[2] += index - 1 ;
    }
    
    //PS: I don't think we will use more than that huge number of machines, no more else if.
    else
    {
        i_address[3] += hosts;
    }

    memset(Broadcast_address, 0, sizeof(Broadcast_address));
    sprintf(Broadcast_address, "%d.%d.%d.%d", i_address[0], i_address[1], i_address[2], i_address[3]);
    return OK;
}
/////////////////////////////////////////////////////

I8 main()
{

    I8 LAN_num = 0;
    I16 input_machines[200 + 1] = {0}; // For now i don't think that someone or company will use more than 200 LAN's , don't dissapoint me //
    
    echo("Enter Netwok address :");
    scanf("%s", Network_address);

    echo("How many LAN's Do you have :");
    scanf("%d",&LAN_num);

    for (I8 i = 0 ; i < LAN_num ; i ++)
    {
        echo("Enter Machine number for %dth LAN : ", i + 1);
        scanf("%d", &input_machines[i]);
    }

    echo("|||||||||||||OUTPUT|||||||||||||||");
    //TODO : sort number of mahcines inputs
    for (I8 lan = 0; lan < LAN_num; lan++)
    {
        //LAN number
        echo("Lan = %d:", lan + 1);
        echo("Number of machines = %d", input_machines[lan]);
        I8 host_bits = Calc_HOSTID(input_machines[lan]);
        echo("host bits = %d", host_bits);
        Calc_NetworkAddress();
        echo("Network Address = %s", Network_address);
        Calc_BroadcastAddress(host_bits);
        echo("Broadcast Address = %s", Broadcast_address);
        I8 prefix = Calc_Prefix(host_bits);
        echo("Prefix = /%d", prefix);
        Calc_mask(prefix);
        echo("Subnet Mask = %s", Mask_address);
        echo("||||||||||||||||||||||||||||||||||");
    }

    return 0;
}