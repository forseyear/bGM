#define bGM_play_midi
{
   /********************************************************************************
    * (ver 2.2.1)
    *
    * bGM_play_midi(filename, is_loop)
    *
    * filenameで指定されたmidiファイルを再生開始する。
    *
    * -string  filename  : 再生するmidiファイル名(midi/midのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    *
    * return        : 再生開始に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : また、bGM_pause関数を使って再生ができる。
    *               : 再生開始に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop;     // 引数をキャッチする変数
   var arg_index;             // 引数走査index
   var cmd_args;              // bGM.exeへ渡すコマンドライン引数

   // ファイル名
   arg_index = 0;
   if (is_string(argument[arg_index]))
   {
      filename = argument[arg_index];
   }
   else
   {
      return false;
   }
   // ループ
   arg_index = 1;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id) +
               " -M";
   if (is_loop)
   {
      cmd_args += " -L";
   }
   if (global.bGM_enable_debugging_console)
   {
      cmd_args += " -e";
   }

   // bGMを実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   return global.bGM_sound_controll_id;
}
