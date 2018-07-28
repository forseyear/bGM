{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_pause(sound_id)
    *
    * サウンドを一時停止させる。
    * あるいは、すでに一時停止状態だった場合、再生を再開させる。
    *
    * -int   sound_id    : 一時停止/再開させたいサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   
   bGM_private_controll_1arg(argument0, "o");
   exit;
}
