#define bGM_is_playing
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_is_playing(sound_id)
    *
    * サウンドの再生状態を確認する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : サウンドを再生しているとtrueを返す。
    *               : 停止中はfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, key;                        // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "y";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_isp_file_path = "isPlayTemp";

   if(!file_exists(global.bGM_isp_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_isp_file_path);
   key = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_isp_file_path);

   // 値を返す
   if(key == 1)
   {
      return true;
   }
   return false;

}
