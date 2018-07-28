{
   /* ------------------- bGM Scriptsの内部から呼ばれるプライベート関数 ------------------- */

   // 再生プロセスへメッセージをポストする

   var sound_id, arg;                             // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = argument1;

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   exit;
}