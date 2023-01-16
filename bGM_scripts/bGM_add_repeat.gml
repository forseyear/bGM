#define bGM_add_repeat
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_add_repeat(sound_id, location, target)
    *
    * サウンドにループポイントを追加する。
    *
    * -int   sound_id                     : 対象のサウンドid (bGM_playの戻り値)
    * -int   location (repeat_point_B)    : ループが発生する場所(0～999,999,999で指定する)(大きい)
    * -int    target  (repeat_point_A)    : ジャンプ先の場所(0～999,999,999で指定する)(小さい)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "a " + string_format(argument1, 9, 0) + " -b " + string_format(argument2, 9, 0));
   exit;

}

