{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_set_loop(sound_id, is_loop)
    *
    * サウンドのループ再生を切り替える。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -bool    is_loop   : ループ再生を行うかどうか
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   if (argument1)
   {
      bGM_private_controll_1arg(argument0, "L");
   }
   else
   {
      bGM_private_controll_1arg(argument0, "u");
   }
   exit;
}
