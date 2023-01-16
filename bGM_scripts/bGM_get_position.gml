#define bGM_get_position
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_get_position(sound_id)
    *
    * サウンドの場所を確認する。再生中に呼び出す場合はラグに注意。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : 成功するとサウンドの場所(int)を返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, number;                     // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "j";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_gpo_file_path = "GetPositionTemp";

   if(!file_exists(global.bGM_gpo_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_gpo_file_path);
   number = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_gpo_file_path);

   // 値を返す
   return number;

}
