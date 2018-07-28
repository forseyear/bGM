{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_fade(sound_id, is_fade, sec, volume)
    *
    * サウンドにフェード効果を追加する。
    *
    * -int     sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -bool    is_fade     : フェードインにはtrue、フェードアウトにはfalseを指定する
    * -int       sec       : フェードの合計時間 (単位はミリ秒。)
    * -double  volume      : 音量(0.0～1.0で指定する。省略可。)
    *                        デフォルト値は、is_fadeがtrueの場合は1.0、falseの場合は0.0
    *                        volumeは、0.000001単位で指定できる。
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   var volume;

   if (is_real(argument3))
   {
      volume = max(0.0, min(1.0, argument3));
   }
   else
   {
      if(argument1)
      {
         volume = 1.000000;
      }
      else
      {
         volume = 0.000000;
      }
   }

   if (argument1)
   {
      bGM_private_controll_1arg(argument0, "V " + string_format(volume, 7, 6) + " -h " + string_format(argument2, 5, 0));
   }
   else
   {
      bGM_private_controll_1arg(argument0, "V " + string_format(volume, 7, 6) + " -q " + string_format(argument2, 5, 0));
   }
   exit;

}
