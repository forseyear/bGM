#define bGM_private_generate_pid
{
   /* ------------------- bGM Scriptsの内部から呼ばれるプライベート関数 ------------------- */

   // サウンド再生を行っているbGMプロセスのIDを返す
   return "GAME=" + string(global.bGM_1time_game_id) + ":SND=" + string(argument0);
}
