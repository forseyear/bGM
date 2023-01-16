#define bGM_play
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_play(filename, volume, is_loop, pan, pitch)
    *
    * filenameで指定された音楽ファイルをvolumeで指定された音量で再生開始する。
    *
    * -string  filename  : 再生するファイル名(mp3/ogg/mod/it/xm/s3mのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -double  volume    : 音量(0.0～1.0で指定する。省略不可)
    *                      ※volume/pan/pitchは、0.000001単位で指定できる。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    * -double  pan       : パン(-1.0で左端、0.0で中央、1.0で右端。省略可。デフォルト値 = 0.0)
    * -double  pitch     : 再生スピード(0.5～2.0で指定する。省略可。デフォルト値 = 1.0)
    *
    * return        : 再生開始に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : 再生開始に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop, volume, pan, pitch;     // 引数をキャッチする変数
   var arg_index;                                 // 引数走査index
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

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

   // ボリューム
   arg_index = 1;
   if (is_real(argument[arg_index]))
   {
      volume = max(0.0, min(1.0, argument[arg_index]));
   }
   else
   {
      volume = 0.5;
   }
   // ループ
   arg_index = 2;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }
   // パン
   arg_index = 3;
   if (is_real(argument[arg_index]))
   {
      pan = max(-1.0, min(1.0, argument[arg_index]));
   }
   else
   {
      pan = 0.0;
   }
   // ピッチシフト
   arg_index = 4;
   if (is_real(argument[arg_index]) && argument[arg_index] != 0)
   {
      pitch = max(0.5, min(2.0, argument[arg_index]));
   }
   else
   {
      pitch = 1.0;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -v " + string_format(volume, 7, 6) +
               " -p " + string_format(pan, 7, 6) +
               " -s " + string_format(pitch, 7, 6) +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id);
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
