#define bGM_set_position
{
   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_set_position(sound_id, position)
    *
    * サウンドの場所を変更させる。
    *
    * -int   sound_id    : 場所を調整したいサウンドid (bGM_playの戻り値)
    * -int   position    : 場所(0～999,999,999で指定する)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "k " + string_format(argument1, 9, 0));
   exit;
}
