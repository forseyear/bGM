{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_is_opening(sound_id)
    *
    * サウンドウィンドウの状態を確認する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : サウンドを開いているとtrueを返す。
    *               : 通常はfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, key;                        // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "n";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_iso_file_path = "isOpenTemp";
    
   if(!file_exists(global.bGM_iso_file_path))
   {
      return false;
   }
   
   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_iso_file_path);
   key = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_iso_file_path);

   // 値を返す
   if(key == 1)
   {
      return true;
   }
   return false;

}
