#define bGM_set_repeat_count
{

   /********************************************************************************
    * (ver 2.1.0)
    *
    * bGM_set_repeat_count(sound_id, repeat_count)
    *
    * サウンドの繰り返し回数を決める。
    *
    * -int   sound_id        : 対象のサウンドid (bGM_playの戻り値)
    * -int   repeat_count    : 繰り返す回数
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "g " + string_format(argument1, 2, 0));
   exit;

}
