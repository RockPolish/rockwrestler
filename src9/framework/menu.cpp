#include "../inc.h"
#include "../tests/tests.h"

extern const char txt_buildtime[];

struct MenuEntry
{
    const char* text;
    void* ptr;
    int type; // 0: test, 1: submenu, 2: function that just prints some stuff (don't print OK afterwards, just wait for B button)
};

class Menu
{
public:
    const MenuEntry* entries;
    const char* title;
    const Menu* parent;

    int get_entry_count() const
    {
        int count = 0;
        const MenuEntry* p = entries;
        while ((p++)->text != nullptr) count++;
        return count;
    }

    void draw() const
    {
        // draw title, buttons
        clear_screen();
        draw_string(3, 0, title);

        for(int i = 0; i < get_entry_count(); i++)
        {
            draw_string(2, 2+i, entries[i].text);
        }

        // only in main menu: draw disclaimer + build time
        if (parent == nullptr)
        {
            draw_string(3, 21, " WIP  CONTACT: ROCKPOLISH");
            draw_string(3, 22, " MAY STILL CONTAIN BUGS  ");
            draw_string(3, 23, txt_buildtime);
        }
    }
};




extern const Menu menu_main, menu_armv4, menu_armv5, menu_ipc, menu_dsmath, menu_memory, menu_initialstate;

const MenuEntry menu_main_entries[] = {
    {"ARMv4", (void*)&menu_armv4, 1},
    {"ARMv5", (void*)&menu_armv5, 1},
    {"IPC", (void*)&menu_ipc, 1},
    {"DS MATH", (void*)&menu_dsmath, 1},
    {"MEMORY", (void*)&menu_memory, 1},
    {"INITIAL STATE", (void*)&menu_initialstate, 1},
    {nullptr, nullptr, 0}
};

const MenuEntry menu_armv4_entries[] = {
    {"CONDITION CODES", (void*)&run_tests_armv4_conditioncodes, 0},
    {nullptr, nullptr, 0}
};

const MenuEntry menu_armv5_entries[] = {
    {"CLZ", (void*)&run_tests_armv5_clz, 0},
    {"QADD, QSUB", (void*)&run_tests_armv5_qadd_qsub, 0},
    {"QDADD, QDSUB", (void*)&run_tests_armv5_qdadd_qdsub, 0},
    {"SMULxy", (void*)&run_tests_armv5_SMULxy, 0},
    {"SMLAxy", (void*)&run_tests_armv5_SMLAxy, 0},
    {"SMULWy", (void*)&run_tests_armv5_SMULWy, 0},
    {"SMLAWy", (void*)&run_tests_armv5_SMLAWy, 0},
    {"SMLALxy", (void*)&run_tests_armv5_SMLALxy, 0},
    {"BLX", (void*)&run_tests_armv5_blx, 0},
    {"LDR r15, POP {r15}, LDM {r15}", (void*)&run_tests_armv5_ldrpopr15, 0},
    {"LDM / STM", (void*)&run_tests_armv5_ldm_stm, 0},
    {nullptr, nullptr, 0}
};


const MenuEntry menu_ipc_entries[] = {
    {"IPCSYNC", (void*)&run_tests_ipcsync, 0},
    {"IPCFIFO", (void*)&run_tests_ipcfifo, 0},
    {"IPCFIFO IRQ", (void*)&run_tests_ipcfifo_irq, 0},
    {nullptr, nullptr, 0}
};

const MenuEntry menu_dsmath_entries[] = {
    {"SQRT 32", (void*)&run_tests_ds_maths_sqrt_32, 0},
    {"SQRT 64", (void*)&run_tests_ds_maths_sqrt_64, 0},
    {"DIV 32/32", (void*)&run_tests_ds_maths_div_32_32, 0},
    {"DIV 64/32", (void*)&run_tests_ds_maths_div_64_32, 0},
    {"DIV 64/64", (void*)&run_tests_ds_maths_div_64_64, 0},
    {nullptr, nullptr, 0}
};

const MenuEntry menu_memory_entries[] = {
    {"WRAM CNT", (void*)&run_tests_wramcnt, 0},
    {"VRAM CNT", (void*)&run_tests_vramcnt, 0},
    {"TCM", (void*)&run_tests_tcm, 0},
    {nullptr, nullptr, 0}
};

const MenuEntry menu_initialstate_entries[] = {
    {"IPC/IRQ/CPSR", (void*)&run_tests_initialstate_ipc_irq_cpsr, 2},
    {"CP15", (void*)&run_tests_initialstate_cp15, 2},
    {nullptr, nullptr, 0}
};

const Menu menu_main = {menu_main_entries, "ROCKWRESTLER", nullptr};
const Menu menu_armv4 = {menu_armv4_entries, "ARMv4", &menu_main};
const Menu menu_armv5 = {menu_armv5_entries, "ARMv5", &menu_main};
const Menu menu_ipc = {menu_ipc_entries, "IPC TESTS", &menu_main};
const Menu menu_dsmath = {menu_dsmath_entries, "DS MATH", &menu_main};
const Menu menu_memory = {menu_memory_entries, "MEMORY CONTROL", &menu_main};
const Menu menu_initialstate = {menu_initialstate_entries, "INITIAL STATE", &menu_main};

const Menu* menu_selected = &menu_main;
int menu_index = 0;

extern "C" void cpp_menu()
{
    menu_selected->draw();

    // how many frames each key has been held, or 0 if currently released
    int key_times[16];
    for(int i = 0; i < 16; i++)
    {
        key_times[i] = 0;
    }

    u16 keys_held = 0, keys_released = 0, keys_held_slow = 0;

    // heartbeat: test if ARM7 is properly in main loop, draw green or red square in top right corner
    int heartbeat_value = 1;
    int heartbeat_time = 0; // frames without response, > 5 -> ARM7 is maybe stuck somewhere (or not implemented, or no IPCSYNC)

    while (true)
    {
        wait_vblank();
        u16 keys_held_prev = keys_held;
        keys_held = ~REG_KEYINPUT;
        // released: held previously, not held now
        keys_released = keys_held_prev & ~keys_held;
        // pressed: held now, not held previously
        u16 keys_pressed = keys_held & ~keys_held_prev;
        keys_held_slow = 0; // keys that are either pressed just now, or are being held for a bit, every n frames
        for(int i = 0; i < 16; i++)
        {
            if ((keys_held >> i) & 1)
            {
                key_times[i]++;
                if ((key_times[i] == 1) || ((key_times[i] > 30) && ((key_times[i] & 7) == 0))) keys_held_slow |= 1 << i;
            }
            else key_times[i] = 0;
        }

        if (keys_held_slow & KEY_MASK_UP)
        {
            draw_cleartile(1, 2+menu_index, 0x56B5);
            menu_index--;
            if (menu_index < 0) menu_index += menu_selected->get_entry_count();
        }
        if (keys_held_slow & KEY_MASK_DOWN)
        {
            draw_cleartile(1, 2+menu_index, 0x56B5);
            menu_index++;
            if (menu_index >= menu_selected->get_entry_count()) menu_index -= menu_selected->get_entry_count();
        }

        if (keys_pressed & KEY_MASK_A)
        {
            clear_screen();

            const MenuEntry& entry = menu_selected->entries[menu_index];
            if (entry.type == 0)
            {
                // test: ptr is a function pointer
                voidfncptr f = (voidfncptr)menu_selected->entries[menu_index].ptr;
                call_fncptr(f); // use call_fncptr so compiler doesn't generate blx
                // if we reach this: the test passed
                draw_string(0, 0, "OK");

                // wait until B button is held
                while (REG_KEYINPUT & KEY_MASK_B) { }
                menu_selected->draw();
                keys_held |= KEY_MASK_B;
                // make the B button pressed so that this B hold does not transfer to the next frame
                // to then be used to go to the previous menu
            }
            else if (entry.type == 1)
            {
                // submenu: ptr is a pointer to a new Menu
                menu_selected = (const Menu*)menu_selected->entries[menu_index].ptr;
                menu_index = 0;
                menu_selected->draw();
            }
            else if (entry.type == 2)
            {
                // just execute the function and then redraw the menu, don't draw OK
                voidfncptr f = (voidfncptr)menu_selected->entries[menu_index].ptr;
                call_fncptr(f);

                while (REG_KEYINPUT & KEY_MASK_B) { }
                menu_selected->draw();
                keys_held |= KEY_MASK_B;
            }
        }

        if (keys_pressed & KEY_MASK_B)
        {
            // go back to previous menu
            if (menu_selected->parent != nullptr)
            {
                const void* from = menu_selected;
                menu_selected = menu_selected->parent;
                // use "from" menu to set cursor correctly
                menu_index = 0;
                while (menu_selected->entries[menu_index].ptr != from) menu_index++;
                menu_selected->draw();
            }
        }

        // draw cursor
        draw_tile(1, 2+menu_index, 0x16);

        // ARM7 heartbeat
        if ((REG_IPCSYNC & 0xF) == heartbeat_value)
        {
            heartbeat_value = 3-heartbeat_value; // alternate 1, 2, 1, 2, ...
            heartbeat_time = 0;
        }
        else heartbeat_time++;

        REG_IPCSYNC = heartbeat_value << 8;

        // top right: "ARM7" + green or red square
        draw_string(27, 0, "ARM7");
        draw_cleartile(31, 0, (heartbeat_time > 5) ? 0x001F : 0x03E0);
    }
}

extern "C" void cpp_fail_test(const char* str, int offset, int number)
{
    // make sure we are in the right VRAM mode etc
    init_iostate();

    // draw the string (fail, timeout, ...)
    draw_string(0, 0, str);

    // + the number of the test that failed (3 hexadecimal digits)
    // timeout_rw doesn't give a test number (-1), so don't draw that
    if (number >= 0) draw_hex_value<3>(offset, 0, number);
}
