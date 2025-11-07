#include"offsets.hpp"
#define offset_gamemode 0x37DD090 //判断模式
#define offset_wanjiashuzu 0x37DD068//玩家数组
#define offset_shuzudaxiao 0x10C8//数组大小
#define offset_bone 0x2958//骨骼偏移
#define offset_benrendizhi 0x2EA //本人ID
#define offset_juzhen 0x1405BC620  //dx_pos//0x140xxxxx矩阵
#define offset_renwujizhi 0x3A6D8D0//人物基址
#define offset_visable 0x1405BD111//257
#define offset_hongming 0x34CA52C//256 257
#define offset_fov 0x3AE2B9C//FOV
#define offset_wuqidizhi 0x39E5C78//武器基址

struct 屏幕坐标
{
    int X;
    int Y;
    int W;
    int H;
};


int 取自己位置()
{
   // return 读整数型(读整数型(游戏基址.玩家数组) + 游戏基址.本人地址) + 1;
    return read<uintptr_t>(read<uintptr_t>(globals::qProcessBase + offset_wanjiashuzu) + offset_benrendizhi)+1;

}

bool 取敌人坐标(int a, Vector b)
{
    int 生存;
    int 敌人;
    uintptr_t shuzu = read<uintptr_t>(globals::qProcessBase + offset_wanjiashuzu);
    生存 = read<uintptr_t>((shuzu + 0x290 + 20) + ((a - 1) * 1840));
    if (生存 == 0)
    {
        return false;
    }
    if (read<int>(生存 + 516) == 0)
    {
        敌人 = read<uintptr_t>((shuzu + 0x290  + (a - 1) * 1840));
        b.x = read<float>(敌人 + 224);
        b.z = read<float>(敌人 + 228);
        b.y = read<float>(敌人 + 232);
        return true;
    }
    else
    {
        return false;
    }

}

int 取人物数量()
{
    uintptr_t address;
    
    
    
    address = read<int>(globals::qProcessBase+offset_gamemode); 
   // printf("address:%d\n", address);
    if (address == 14)
    {
        aim::生化模式 = true;
    }
    else
        aim::生化模式 = false;
    if (address == 11 || address == 24 || address == 31) {
        return 0;
    }
    else if (address == 15) {
        return 5;
    }
    else if (address == 5 || address == 12 || address == 14 || address == 17 || address == 41 || address == 47) {
        return 15;
    }
    else if (address == 4 || address == 36) {
        return 16;
    }

    return 8;
}

int 取人物位置()
{
    if ((read<uintptr_t>(read<uintptr_t>(globals::qProcessBase + offset_wanjiashuzu) + 488) + 1) <= 8)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



void 取骨骼坐标(DWORD ID, Vector& 寄存, DWORD 位置, uintptr_t pEntity, uintptr_t xx)
{
    if (pEntity > 0)
    {

        if (xx > 0)
        {
            xx = xx + (12 + 64 * 位置);
            
            寄存.x = scatterRead<float>(globals::hS, xx);
            寄存.z = scatterRead<float>(globals::hS, xx + 16);
            寄存.y = scatterRead<float>(globals::hS, xx + 32);

        }
    }
}

void 取自瞄坐标(DWORD ID, Vector& 寄存, DWORD 位置, uintptr_t pEntity, uintptr_t xx)
{
    if (pEntity > 0)
    {

        if (xx > 0)
        {
            xx = xx + (12 + 64 * 位置);

            寄存.x = read<float>( xx);
            寄存.z = read<float>(xx + 16);
            寄存.y = read<float>( xx + 32);

        }
    }
}




int 取生化状态(DWORD ID)
{
    return read<int>(read<int>(globals::qProcessBase + offset_wanjiashuzu + 20 + (ID - 1) * offset_shuzudaxiao) + 314660);
}

bool cfworldscreen(Vector a, Vector& back)
{

    //  byte 观察[64] = { NULL };
    //  byte 投影[64] = { NULL };
    //  byte 视口[64] = { NULL };

    globals::敌人.x = a.x;
    globals::敌人.y = a.z;
    globals::敌人.z = a.y;

    // 视口[64] = readarray<byte>(offset_juzhen, 视口, 64);
    // 观察[64] = readarray<byte>(offset_juzhen - 128, 观察, 64);
    // 投影[64] = readarray<byte>(offset_juzhen - 64, 投影, 64);

    D3DXVec3Project(&globals::镜像, &globals::敌人, (D3DVIEWPORT9*)&globals::视口, (D3DXMATRIX*)&globals::投影, (D3DXMATRIX*)&globals::观察, (D3DXMATRIX*)&globals::世界);

    if (globals::镜像.z > 1)
    {
        return false;
    }
    else
    {
        back.x = globals::镜像.x;
        back.y = globals::镜像.y;
        return true;
    }

}
static DWORD aimbot_ms;
void startAim(Vector AimBonepos)
{
    

    float x = AimBonepos.x-ScreenWidth/2;
    float y = AimBonepos.y-ScreenHeight / 2;
 
    float smooth_x = 0.00f;
    float smooth_y = 0.00f;

    DWORD ms = 0;
    if (aim::factorx >= 1.0f)
    {
        if (qabs(x) > 1.0f)
        {
            if (smooth_x < x)
                smooth_x = smooth_x + 1.0f + (x / aim::factorx);
            else if (smooth_x > x)
                smooth_x = smooth_x - 1.0f + (x / aim::factorx);
            else
                smooth_x = x;
        }
        else
        {
            smooth_x = x;
        }

        if (qabs(y) > 1.0f)
        {
            if (smooth_y < y)
                smooth_y = smooth_y + 1.0f + (y / aim::factory);
            else if (smooth_y > y)
                smooth_y = smooth_y - 1.0f + (y / aim::factory);
            else
                smooth_y = y;
        }
        else
        {
            smooth_y = y;
        }
        ms = (DWORD)(aim::factorx / 100.0f) + 1;
        ms = ms * 8;
    }
    else
    {
        smooth_x = x;
        smooth_y = y;
        ms = 8;
    }
    DWORD current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (current_ms - aimbot_ms >= ms)
    {
        aimbot_ms = current_ms;
        if (aim::addr > 0 && aim::screenfov <= aim::fov && aim::aimbot)
        {
            if (globals::km)
            {
                km_move((int)smooth_x, (int)smooth_y);
            }

            if (globals::kmnet)
            {
                kmNet_mouse_move_auto((int)smooth_x, (int)smooth_y, 8);
            }

        }
        if (aim::addr > 0 && aim::screenfov <= aim::fov && aim::红名吸附)
        {
            if (globals::km)
            {
                km_move((int)smooth_x, (int)smooth_y);
            }

            if (globals::kmnet)
            {
                kmNet_mouse_move_auto((int)smooth_x, (int)smooth_y, 8);
            }

        }
    }

}