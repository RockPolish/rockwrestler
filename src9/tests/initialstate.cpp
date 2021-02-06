#include "../inc.h"

u16 val_IPCSYNC, val_IPCFIFOCNT, FIFO_length;
u32 val_IME, val_IE, val_IF;

vu32* const result_base = (vu32*)0x2000004; // where the ARM7 has written some results

// function that is called once at the start, should store the results somewhere
extern "C" void test_initialstate()
{
    l_DIC();
    // get some values of registers at boot time, store these in the global vars defined above, print them in run_tests_initialstate()
    val_IPCSYNC = REG_IPCSYNC;
    val_IPCFIFOCNT = REG_IPCFIFOCNT;

    // enable FIFO + clear error bit
    REG_IPCFIFOCNT = (1 << 15) | (1 << 14);
    // count how many values are in the queue (this also clears the queue)
    FIFO_length = 0;
    while (!(REG_IPCFIFOCNT & (1 << 8))) // as long as the FIFO is not empty
    {
        vu32 FIFO_val = REG_IPCFIFORECV;
        FIFO_length++;
        if (FIFO_length > 20)
        {
            // IPCFIFO maybe unimplemented, shouldn't keep looping here forever
            FIFO_length = 0xFFFF;
            break;
        }
    }
    val_IME = REG_IME;
    val_IE = REG_IE;
    val_IF = REG_IF;
    REG_IME = 0;
    REG_IE = 0;
    REG_IF = 0xFFFFFFFF; // acknowledge all
}

extern u32 val_CPSR;

// function that prints the results
void run_tests_initialstate_ipc_irq_cpsr()
{
    draw_string(15, 1, "ARM7");
    draw_string(24, 1, "ARM9");

    draw_string(2, 3, "IPCSYNC");
    draw_hex_value<4>(17, 3, result_base[0]);
    draw_hex_value<4>(26, 3, val_IPCSYNC);

    draw_string(2, 4, "IPCFIFOCNT");
    draw_hex_value<4>(17, 4, result_base[1]);
    draw_hex_value<4>(26, 4, val_IPCFIFOCNT);

    draw_string(2, 5, "FIFO LEN");
    draw_hex_value<4>(17, 5, result_base[2]);
    draw_hex_value<4>(26, 5, FIFO_length);

    draw_string(2, 7, "IME");
    draw_hex_value<1>(20, 7, result_base[3]);
    draw_hex_value<1>(29, 7, val_IME);

    draw_string(2, 8, "IE");
    draw_hex_value<8>(13, 8, result_base[4]);
    draw_hex_value<8>(22, 8, val_IE);

    draw_string(2, 9, "IF");
    draw_hex_value<8>(13, 9, result_base[5]);
    draw_hex_value<8>(22, 9, val_IF);

    draw_string(2, 11, "CPSR");
    draw_hex_value<8>(13, 11, result_base[6]);
    draw_hex_value<8>(22, 11, val_CPSR);
}

extern u32 val_cp15[];

void run_tests_initialstate_cp15()
{
    const char* labels[] = {
        "C1,C0,0",
        "C2,C0,0",
        "C2,C0,1",
        "C3,C0,0",
        "C5,C0,0",
        "C5,C0,1",
        "C5,C0,2",
        "C5,C0,3",
        "C6,C0,0",
        "C6,C1,0",
        "C6,C2,0",
        "C6,C3,0",
        "C6,C4,0",
        "C6,C5,0",
        "C6,C6,0",
        "C6,C7,0",
        "C9,C1,0",
        "C9,C1,1"
    };

    int j = 1;
    for(int i = 0; i < 18; i++)
    {
        draw_string(2, j, labels[i]);
        draw_hex_value<8>(10, j, val_cp15[i]);
        j++;
        if ((i == 3) | (i == 7) | (i == 15)) j++;
    }
}
