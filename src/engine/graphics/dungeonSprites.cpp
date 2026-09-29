#include "engine/graphics/dungeonSprites.h"

namespace {
const SDL_FRect frames_big_demon_idle[] = { {16.0f, 428.0f, 32.0f, 36.0f}, {48.0f, 428.0f, 32.0f, 36.0f}, {80.0f, 428.0f, 32.0f, 36.0f}, {112.0f, 428.0f, 32.0f, 36.0f} };
const SDL_FRect frames_big_demon_run[] = { {144.0f, 428.0f, 32.0f, 36.0f}, {176.0f, 428.0f, 32.0f, 36.0f}, {208.0f, 428.0f, 32.0f, 36.0f}, {240.0f, 428.0f, 32.0f, 36.0f} };
const SDL_FRect frames_big_zombie_idle[] = { {16.0f, 332.0f, 32.0f, 36.0f}, {48.0f, 332.0f, 32.0f, 36.0f}, {80.0f, 332.0f, 32.0f, 36.0f}, {112.0f, 332.0f, 32.0f, 36.0f} };
const SDL_FRect frames_big_zombie_run[] = { {144.0f, 332.0f, 32.0f, 36.0f}, {176.0f, 332.0f, 32.0f, 36.0f}, {208.0f, 332.0f, 32.0f, 36.0f}, {240.0f, 332.0f, 32.0f, 36.0f} };
const SDL_FRect frames_bomb[] = { {288.0f, 320.0f, 16.0f, 16.0f}, {304.0f, 320.0f, 16.0f, 16.0f}, {320.0f, 320.0f, 16.0f, 16.0f} };
const SDL_FRect frames_chest_empty_open[] = { {304.0f, 400.0f, 16.0f, 16.0f}, {320.0f, 400.0f, 16.0f, 16.0f}, {336.0f, 400.0f, 16.0f, 16.0f} };
const SDL_FRect frames_chest_full_open[] = { {304.0f, 416.0f, 16.0f, 16.0f}, {320.0f, 416.0f, 16.0f, 16.0f}, {336.0f, 416.0f, 16.0f, 16.0f} };
const SDL_FRect frames_chest_mimic_open[] = { {304.0f, 432.0f, 16.0f, 16.0f}, {320.0f, 432.0f, 16.0f, 16.0f}, {336.0f, 432.0f, 16.0f, 16.0f} };
const SDL_FRect frames_coin[] = { {289.0f, 385.0f, 6.0f, 7.0f}, {297.0f, 385.0f, 6.0f, 7.0f}, {305.0f, 385.0f, 6.0f, 7.0f}, {313.0f, 385.0f, 6.0f, 7.0f} };
const SDL_FRect frames_flask_big_blue[] = { {304.0f, 336.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_big_red[] = { {288.0f, 336.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_big_green[] = { {320.0f, 336.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_big_yellow[] = { {336.0f, 336.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_blue[] = { {304.0f, 352.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_red[] = { {288.0f, 352.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_green[] = { {320.0f, 352.0f, 16.0f, 16.0f} };
const SDL_FRect frames_flask_yellow[] = { {336.0f, 352.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wizzard_f_hit[] = { {256.0f, 132.0f, 16.0f, 28.0f} };
const SDL_FRect frames_wizzard_f_idle[] = { {128.0f, 132.0f, 16.0f, 28.0f}, {144.0f, 132.0f, 16.0f, 28.0f}, {160.0f, 132.0f, 16.0f, 28.0f}, {176.0f, 132.0f, 16.0f, 28.0f} };
const SDL_FRect frames_wizzard_f_run[] = { {192.0f, 132.0f, 16.0f, 28.0f}, {208.0f, 132.0f, 16.0f, 28.0f}, {224.0f, 132.0f, 16.0f, 28.0f}, {240.0f, 132.0f, 16.0f, 28.0f} };
const SDL_FRect frames_wizzard_m_hit[] = { {256.0f, 164.0f, 16.0f, 28.0f} };
const SDL_FRect frames_wizzard_m_idle[] = { {128.0f, 164.0f, 16.0f, 28.0f}, {144.0f, 164.0f, 16.0f, 28.0f}, {160.0f, 164.0f, 16.0f, 28.0f}, {176.0f, 164.0f, 16.0f, 28.0f} };
const SDL_FRect frames_wizzard_m_run[] = { {192.0f, 164.0f, 16.0f, 28.0f}, {208.0f, 164.0f, 16.0f, 28.0f}, {224.0f, 164.0f, 16.0f, 28.0f}, {240.0f, 164.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_m_idle[] = { {128.0f, 292.0f, 16.0f, 28.0f}, {144.0f, 292.0f, 16.0f, 28.0f}, {160.0f, 292.0f, 16.0f, 28.0f}, {176.0f, 292.0f, 16.0f, 28.0f} };
const SDL_FRect frames_imp_idle[] = { {368.0f, 64.0f, 16.0f, 16.0f}, {384.0f, 64.0f, 16.0f, 16.0f}, {400.0f, 64.0f, 16.0f, 16.0f}, {416.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_imp_run[] = { {432.0f, 64.0f, 16.0f, 16.0f}, {448.0f, 64.0f, 16.0f, 16.0f}, {464.0f, 64.0f, 16.0f, 16.0f}, {480.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_lizard_f_hit[] = { {256.0f, 196.0f, 16.0f, 28.0f} };
const SDL_FRect frames_lizard_f_idle[] = { {128.0f, 196.0f, 16.0f, 28.0f}, {144.0f, 196.0f, 16.0f, 28.0f}, {160.0f, 196.0f, 16.0f, 28.0f}, {176.0f, 196.0f, 16.0f, 28.0f} };
const SDL_FRect frames_lizard_f_run[] = { {192.0f, 196.0f, 16.0f, 28.0f}, {208.0f, 196.0f, 16.0f, 28.0f}, {224.0f, 196.0f, 16.0f, 28.0f}, {240.0f, 196.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_m_run[] = { {192.0f, 292.0f, 16.0f, 28.0f}, {208.0f, 292.0f, 16.0f, 28.0f}, {224.0f, 292.0f, 16.0f, 28.0f}, {240.0f, 292.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_m_hit[] = { {256.0f, 292.0f, 16.0f, 28.0f} };
const SDL_FRect frames_knight_f_hit[] = { {256.0f, 68.0f, 16.0f, 28.0f} };
const SDL_FRect frames_knight_f_idle[] = { {128.0f, 68.0f, 16.0f, 28.0f}, {144.0f, 68.0f, 16.0f, 28.0f}, {160.0f, 68.0f, 16.0f, 28.0f}, {176.0f, 68.0f, 16.0f, 28.0f} };
const SDL_FRect frames_knight_f_run[] = { {192.0f, 68.0f, 16.0f, 28.0f}, {208.0f, 68.0f, 16.0f, 28.0f}, {224.0f, 68.0f, 16.0f, 28.0f}, {240.0f, 68.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_f_idle[] = { {128.0f, 260.0f, 16.0f, 28.0f}, {144.0f, 260.0f, 16.0f, 28.0f}, {160.0f, 260.0f, 16.0f, 28.0f}, {176.0f, 260.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_f_run[] = { {192.0f, 260.0f, 16.0f, 28.0f}, {208.0f, 260.0f, 16.0f, 28.0f}, {224.0f, 260.0f, 16.0f, 28.0f}, {240.0f, 260.0f, 16.0f, 28.0f} };
const SDL_FRect frames_dwarf_f_hit[] = { {256.0f, 260.0f, 16.0f, 28.0f} };
const SDL_FRect frames_weapon_anime_sword[] = { {322.0f, 65.0f, 12.0f, 30.0f} };
const SDL_FRect frames_weapon_arrow[] = { {324.0f, 202.0f, 7.0f, 21.0f} };
const SDL_FRect frames_weapon_baton_with_spikes[] = { {323.0f, 41.0f, 10.0f, 22.0f} };
const SDL_FRect frames_weapon_big_hammer[] = { {291.0f, 26.0f, 10.0f, 37.0f} };
const SDL_FRect frames_weapon_bow[] = { {289.0f, 195.0f, 14.0f, 26.0f} };
const SDL_FRect frames_weapon_bow_2[] = { {305.0f, 195.0f, 14.0f, 26.0f} };
const SDL_FRect frames_weapon_double_axe[] = { {288.0f, 167.0f, 16.0f, 24.0f} };
const SDL_FRect frames_weapon_cleaver[] = { {310.0f, 108.0f, 9.0f, 19.0f} };
const SDL_FRect frames_weapon_duel_sword[] = { {325.0f, 97.0f, 9.0f, 30.0f} };
const SDL_FRect frames_weapon_golden_sword[] = { {291.0f, 137.0f, 10.0f, 22.0f} };
const SDL_FRect frames_weapon_green_magic_staff[] = { {340.0f, 129.0f, 8.0f, 30.0f} };
const SDL_FRect frames_weapon_hammer[] = { {307.0f, 39.0f, 10.0f, 24.0f} };
const SDL_FRect frames_weapon_katana[] = { {293.0f, 66.0f, 6.0f, 29.0f} };
const SDL_FRect frames_weapon_knife[] = { {293.0f, 10.0f, 6.0f, 13.0f} };
const SDL_FRect frames_weapon_knight_sword[] = { {339.0f, 98.0f, 10.0f, 29.0f} };
const SDL_FRect frames_weapon_lavish_sword[] = { {307.0f, 129.0f, 10.0f, 30.0f} };
const SDL_FRect frames_weapon_mace[] = { {339.0f, 39.0f, 10.0f, 24.0f} };
const SDL_FRect frames_weapon_saw_sword[] = { {307.0f, 70.0f, 10.0f, 25.0f} };
const SDL_FRect frames_weapon_machete[] = { {294.0f, 105.0f, 5.0f, 22.0f} };
const SDL_FRect frames_weapon_red_gem_sword[] = { {339.0f, 10.0f, 10.0f, 21.0f} };
const SDL_FRect frames_weapon_red_magic_staff[] = { {324.0f, 129.0f, 8.0f, 30.0f} };
const SDL_FRect frames_weapon_regular_sword[] = { {323.0f, 10.0f, 10.0f, 21.0f} };
const SDL_FRect frames_weapon_rusty_sword[] = { {307.0f, 10.0f, 10.0f, 21.0f} };
const SDL_FRect frames_weapon_spear[] = { {309.0f, 161.0f, 6.0f, 30.0f} };
const SDL_FRect frames_weapon_waraxe[] = { {324.0f, 168.0f, 12.0f, 23.0f} };
const SDL_FRect frames_weapon_throwing_axe[] = { {340.0f, 161.0f, 10.0f, 14.0f} };
const SDL_FRect frames_weapon_axe[] = { {341.0f, 74.0f, 9.0f, 21.0f} };
const SDL_FRect frames_wogol_idle[] = { {368.0f, 249.0f, 16.0f, 23.0f}, {384.0f, 249.0f, 16.0f, 23.0f}, {400.0f, 249.0f, 16.0f, 23.0f}, {416.0f, 249.0f, 16.0f, 23.0f} };
const SDL_FRect frames_wogol_run[] = { {432.0f, 249.0f, 16.0f, 23.0f}, {448.0f, 249.0f, 16.0f, 23.0f}, {464.0f, 249.0f, 16.0f, 23.0f}, {480.0f, 249.0f, 16.0f, 23.0f} };
const SDL_FRect frames_zombie[] = { {368.0f, 136.0f, 16.0f, 16.0f}, {384.0f, 136.0f, 16.0f, 16.0f}, {400.0f, 136.0f, 16.0f, 16.0f}, {416.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_tiny_zombie_idle[] = { {368.0f, 16.0f, 16.0f, 16.0f}, {384.0f, 16.0f, 16.0f, 16.0f}, {400.0f, 16.0f, 16.0f, 16.0f}, {416.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_tiny_zombie_run[] = { {432.0f, 16.0f, 16.0f, 16.0f}, {448.0f, 16.0f, 16.0f, 16.0f}, {464.0f, 16.0f, 16.0f, 16.0f}, {480.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_goblin_idle[] = { {368.0f, 40.0f, 16.0f, 16.0f}, {384.0f, 40.0f, 16.0f, 16.0f}, {400.0f, 40.0f, 16.0f, 16.0f}, {416.0f, 40.0f, 16.0f, 16.0f} };
const SDL_FRect frames_goblin_run[] = { {432.0f, 40.0f, 16.0f, 16.0f}, {448.0f, 40.0f, 16.0f, 16.0f}, {464.0f, 40.0f, 16.0f, 16.0f}, {480.0f, 40.0f, 16.0f, 16.0f} };
const SDL_FRect frames_ice_zombie[] = { {432.0f, 136.0f, 16.0f, 16.0f}, {448.0f, 136.0f, 16.0f, 16.0f}, {464.0f, 136.0f, 16.0f, 16.0f}, {480.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_knight_m_idle[] = { {128.0f, 100.0f, 16.0f, 28.0f}, {144.0f, 100.0f, 16.0f, 28.0f}, {160.0f, 100.0f, 16.0f, 28.0f}, {176.0f, 100.0f, 16.0f, 28.0f} };
const SDL_FRect frames_knight_m_run[] = { {192.0f, 100.0f, 16.0f, 28.0f}, {208.0f, 100.0f, 16.0f, 28.0f}, {224.0f, 100.0f, 16.0f, 28.0f}, {240.0f, 100.0f, 16.0f, 28.0f} };
const SDL_FRect frames_knight_m_hit[] = { {256.0f, 100.0f, 16.0f, 28.0f} };
const SDL_FRect frames_crate[] = { {288.0f, 408.0f, 16.0f, 24.0f} };
const SDL_FRect frames_edge_down[] = { {96.0f, 128.0f, 16.0f, 16.0f} };
const SDL_FRect frames_orc_shaman_idle[] = { {368.0f, 201.0f, 16.0f, 23.0f}, {384.0f, 201.0f, 16.0f, 23.0f}, {400.0f, 201.0f, 16.0f, 23.0f}, {416.0f, 201.0f, 16.0f, 23.0f} };
const SDL_FRect frames_orc_shaman_run[] = { {432.0f, 201.0f, 16.0f, 23.0f}, {448.0f, 201.0f, 16.0f, 23.0f}, {464.0f, 201.0f, 16.0f, 23.0f}, {480.0f, 201.0f, 16.0f, 23.0f} };
const SDL_FRect frames_swampy[] = { {432.0f, 112.0f, 16.0f, 16.0f}, {448.0f, 112.0f, 16.0f, 16.0f}, {464.0f, 112.0f, 16.0f, 16.0f}, {480.0f, 112.0f, 16.0f, 16.0f} };
const SDL_FRect frames_muddy[] = { {368.0f, 112.0f, 16.0f, 16.0f}, {384.0f, 112.0f, 16.0f, 16.0f}, {400.0f, 112.0f, 16.0f, 16.0f}, {416.0f, 112.0f, 16.0f, 16.0f} };
const SDL_FRect frames_necromancer[] = { {368.0f, 225.0f, 16.0f, 23.0f}, {384.0f, 225.0f, 16.0f, 23.0f}, {400.0f, 225.0f, 16.0f, 23.0f}, {416.0f, 225.0f, 16.0f, 23.0f} };
const SDL_FRect frames_masked_orc_idle[] = { {368.0f, 153.0f, 16.0f, 23.0f}, {384.0f, 153.0f, 16.0f, 23.0f}, {400.0f, 153.0f, 16.0f, 23.0f}, {416.0f, 153.0f, 16.0f, 23.0f} };
const SDL_FRect frames_masked_orc_run[] = { {432.0f, 153.0f, 16.0f, 23.0f}, {448.0f, 153.0f, 16.0f, 23.0f}, {464.0f, 153.0f, 16.0f, 23.0f}, {480.0f, 153.0f, 16.0f, 23.0f} };
const SDL_FRect frames_orc_warrior_idle[] = { {368.0f, 177.0f, 16.0f, 23.0f}, {384.0f, 177.0f, 16.0f, 23.0f}, {400.0f, 177.0f, 16.0f, 23.0f}, {416.0f, 177.0f, 16.0f, 23.0f} };
const SDL_FRect frames_orc_warrior_run[] = { {432.0f, 177.0f, 16.0f, 23.0f}, {448.0f, 177.0f, 16.0f, 23.0f}, {464.0f, 177.0f, 16.0f, 23.0f}, {480.0f, 177.0f, 16.0f, 23.0f} };
const SDL_FRect frames_skelet_idle[] = { {368.0f, 88.0f, 16.0f, 16.0f}, {384.0f, 88.0f, 16.0f, 16.0f}, {400.0f, 88.0f, 16.0f, 16.0f}, {416.0f, 88.0f, 16.0f, 16.0f} };
const SDL_FRect frames_skelet_run[] = { {432.0f, 88.0f, 16.0f, 16.0f}, {448.0f, 88.0f, 16.0f, 16.0f}, {464.0f, 88.0f, 16.0f, 16.0f}, {480.0f, 88.0f, 16.0f, 16.0f} };
const SDL_FRect frames_skull[] = { {288.0f, 432.0f, 16.0f, 16.0f} };
const SDL_FRect frames_ogre_idle[] = { {16.0f, 380.0f, 32.0f, 36.0f}, {48.0f, 380.0f, 32.0f, 36.0f}, {80.0f, 380.0f, 32.0f, 36.0f}, {112.0f, 380.0f, 32.0f, 36.0f} };
const SDL_FRect frames_ogre_run[] = { {144.0f, 380.0f, 32.0f, 36.0f}, {176.0f, 380.0f, 32.0f, 36.0f}, {208.0f, 380.0f, 32.0f, 36.0f}, {240.0f, 380.0f, 32.0f, 36.0f} };
const SDL_FRect frames_ui_heart_empty[] = { {321.0f, 370.0f, 13.0f, 12.0f} };
const SDL_FRect frames_ui_heart_full[] = { {289.0f, 370.0f, 13.0f, 12.0f} };
const SDL_FRect frames_ui_heart_half[] = { {305.0f, 370.0f, 13.0f, 12.0f} };
const SDL_FRect frames_doc_idle[] = { {368.0f, 345.0f, 16.0f, 23.0f}, {384.0f, 345.0f, 16.0f, 23.0f}, {400.0f, 345.0f, 16.0f, 23.0f}, {416.0f, 345.0f, 16.0f, 23.0f} };
const SDL_FRect frames_doc_run[] = { {432.0f, 345.0f, 16.0f, 23.0f}, {448.0f, 345.0f, 16.0f, 23.0f}, {464.0f, 345.0f, 16.0f, 23.0f}, {480.0f, 345.0f, 16.0f, 23.0f} };
const SDL_FRect frames_pumpkin_dude_idle[] = { {368.0f, 321.0f, 16.0f, 23.0f}, {384.0f, 321.0f, 16.0f, 23.0f}, {400.0f, 321.0f, 16.0f, 23.0f}, {416.0f, 321.0f, 16.0f, 23.0f} };
const SDL_FRect frames_pumpkin_dude_run[] = { {432.0f, 321.0f, 16.0f, 23.0f}, {448.0f, 321.0f, 16.0f, 23.0f}, {464.0f, 321.0f, 16.0f, 23.0f}, {480.0f, 321.0f, 16.0f, 23.0f} };
const SDL_FRect frames_angel_idle[] = { {368.0f, 304.0f, 16.0f, 16.0f}, {384.0f, 304.0f, 16.0f, 16.0f}, {400.0f, 304.0f, 16.0f, 16.0f}, {416.0f, 304.0f, 16.0f, 16.0f} };
const SDL_FRect frames_angel_run[] = { {432.0f, 304.0f, 16.0f, 16.0f}, {448.0f, 304.0f, 16.0f, 16.0f}, {464.0f, 304.0f, 16.0f, 16.0f}, {480.0f, 304.0f, 16.0f, 16.0f} };
const SDL_FRect frames_chort_idle[] = { {368.0f, 273.0f, 16.0f, 23.0f}, {384.0f, 273.0f, 16.0f, 23.0f}, {400.0f, 273.0f, 16.0f, 23.0f}, {416.0f, 273.0f, 16.0f, 23.0f} };
const SDL_FRect frames_chort_run[] = { {432.0f, 273.0f, 16.0f, 23.0f}, {448.0f, 273.0f, 16.0f, 23.0f}, {464.0f, 273.0f, 16.0f, 23.0f}, {480.0f, 273.0f, 16.0f, 23.0f} };
const SDL_FRect frames_column[] = { {80.0f, 80.0f, 16.0f, 48.0f} };
const SDL_FRect frames_column_wall[] = { {96.0f, 80.0f, 16.0f, 48.0f} };
const SDL_FRect frames_wall_fountain_mid_blue[] = { {64.0f, 48.0f, 16.0f, 16.0f}, {80.0f, 48.0f, 16.0f, 16.0f}, {96.0f, 48.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_mid_red[] = { {64.0f, 16.0f, 16.0f, 16.0f}, {80.0f, 16.0f, 16.0f, 16.0f}, {96.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_basin_red[] = { {64.0f, 32.0f, 16.0f, 16.0f}, {80.0f, 32.0f, 16.0f, 16.0f}, {96.0f, 32.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_basin_blue[] = { {64.0f, 64.0f, 16.0f, 16.0f}, {80.0f, 64.0f, 16.0f, 16.0f}, {96.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_goo_base[] = { {64.0f, 96.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_goo[] = { {64.0f, 80.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_hole_1[] = { {48.0f, 32.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_hole_2[] = { {48.0f, 48.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_banner_blue[] = { {32.0f, 32.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_banner_red[] = { {16.0f, 32.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_banner_green[] = { {16.0f, 48.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_banner_yellow[] = { {32.0f, 48.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_ladder[] = { {48.0f, 96.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_spikes[] = { {16.0f, 192.0f, 16.0f, 16.0f}, {32.0f, 192.0f, 16.0f, 16.0f}, {48.0f, 192.0f, 16.0f, 16.0f}, {64.0f, 192.0f, 16.0f, 16.0f} };
const SDL_FRect frames_hole[] = { {96.0f, 144.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_stairs[] = { {80.0f, 192.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_1[] = { {16.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_2[] = { {32.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_3[] = { {48.0f, 64.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_4[] = { {16.0f, 80.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_5[] = { {32.0f, 80.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_6[] = { {48.0f, 80.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_7[] = { {16.0f, 96.0f, 16.0f, 16.0f} };
const SDL_FRect frames_floor_8[] = { {32.0f, 96.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_left[] = { {16.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_mid[] = { {32.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_right[] = { {48.0f, 16.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_top_left[] = { {16.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_top_mid[] = { {32.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_top_right[] = { {48.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_lizard_m_idle[] = { {128.0f, 228.0f, 16.0f, 28.0f}, {144.0f, 228.0f, 16.0f, 28.0f}, {160.0f, 228.0f, 16.0f, 28.0f}, {176.0f, 228.0f, 16.0f, 28.0f} };
const SDL_FRect frames_lizard_m_run[] = { {192.0f, 228.0f, 16.0f, 28.0f}, {208.0f, 228.0f, 16.0f, 28.0f}, {224.0f, 228.0f, 16.0f, 28.0f}, {240.0f, 228.0f, 16.0f, 28.0f} };
const SDL_FRect frames_lizard_m_hit[] = { {256.0f, 228.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_f_idle[] = { {128.0f, 4.0f, 16.0f, 28.0f}, {144.0f, 4.0f, 16.0f, 28.0f}, {160.0f, 4.0f, 16.0f, 28.0f}, {176.0f, 4.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_f_run[] = { {192.0f, 4.0f, 16.0f, 28.0f}, {208.0f, 4.0f, 16.0f, 28.0f}, {224.0f, 4.0f, 16.0f, 28.0f}, {240.0f, 4.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_f_hit[] = { {256.0f, 4.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_m_idle[] = { {128.0f, 36.0f, 16.0f, 28.0f}, {144.0f, 36.0f, 16.0f, 28.0f}, {160.0f, 36.0f, 16.0f, 28.0f}, {176.0f, 36.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_m_run[] = { {192.0f, 36.0f, 16.0f, 28.0f}, {208.0f, 36.0f, 16.0f, 28.0f}, {224.0f, 36.0f, 16.0f, 28.0f}, {240.0f, 36.0f, 16.0f, 28.0f} };
const SDL_FRect frames_elf_m_hit[] = { {256.0f, 36.0f, 16.0f, 28.0f} };
const SDL_FRect frames_button_red_up[] = { {16.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_button_red_down[] = { {32.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_button_blue_up[] = { {48.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_button_blue_down[] = { {64.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_lever_left[] = { {80.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_lever_right[] = { {96.0f, 208.0f, 16.0f, 16.0f} };
const SDL_FRect frames_doors_frame_left[] = { {16.0f, 240.0f, 16.0f, 32.0f} };
const SDL_FRect frames_doors_frame_right[] = { {64.0f, 240.0f, 16.0f, 32.0f} };
const SDL_FRect frames_doors_frame_top[] = { {32.0f, 224.0f, 32.0f, 16.0f} };
const SDL_FRect frames_doors_leaf_closed[] = { {32.0f, 240.0f, 32.0f, 32.0f} };
const SDL_FRect frames_doors_leaf_open[] = { {80.0f, 240.0f, 32.0f, 32.0f} };
const SDL_FRect frames_wall_edge_bottom_left[] = { {32.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_bottom_right[] = { {48.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_mid_left[] = { {32.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_top_left[] = { {31.0f, 120.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_left[] = { {32.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_top_right[] = { {48.0f, 120.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_right[] = { {48.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_mid_right[] = { {48.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_tshape_bottom_right[] = { {64.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_tshape_bottom_left[] = { {80.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_tshape_right[] = { {64.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_edge_tshape_left[] = { {80.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_front_right[] = { {16.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_front_left[] = { {0.0f, 168.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_mid_left[] = { {0.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_top_left[] = { {0.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_top_right[] = { {16.0f, 136.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_outer_mid_right[] = { {16.0f, 152.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_top_1[] = { {64.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_top_2[] = { {80.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_wall_fountain_top_3[] = { {96.0f, 0.0f, 16.0f, 16.0f} };
const SDL_FRect frames_slug[] = { {368.0f, 369.0f, 16.0f, 23.0f}, {384.0f, 369.0f, 16.0f, 23.0f}, {400.0f, 369.0f, 16.0f, 23.0f}, {416.0f, 369.0f, 16.0f, 23.0f} };
const SDL_FRect frames_tiny_slug[] = { {432.0f, 376.0f, 16.0f, 16.0f}, {448.0f, 376.0f, 16.0f, 16.0f}, {464.0f, 376.0f, 16.0f, 16.0f}, {480.0f, 376.0f, 16.0f, 16.0f} };
}

const DungeonSpriteEntry DUNGEON_SPRITE_TABLE[DUNGEON_SPRITE_COUNT] = {
    { frames_big_demon_idle, 4, 16, 18 },
    { frames_big_demon_run, 4, 16, 18 },
    { frames_big_zombie_idle, 4, 16, 18 },
    { frames_big_zombie_run, 4, 16, 18 },
    { frames_bomb, 3, 8, 8 },
    { frames_chest_empty_open, 3, 8, 8 },
    { frames_chest_full_open, 3, 8, 8 },
    { frames_chest_mimic_open, 3, 8, 8 },
    { frames_coin, 4, 3, 3 },
    { frames_flask_big_blue, 1, 8, 8 },
    { frames_flask_big_red, 1, 8, 8 },
    { frames_flask_big_green, 1, 8, 8 },
    { frames_flask_big_yellow, 1, 8, 8 },
    { frames_flask_blue, 1, 8, 8 },
    { frames_flask_red, 1, 8, 8 },
    { frames_flask_green, 1, 8, 8 },
    { frames_flask_yellow, 1, 8, 8 },
    { frames_wizzard_f_hit, 1, 8, 14 },
    { frames_wizzard_f_idle, 4, 8, 14 },
    { frames_wizzard_f_run, 4, 8, 14 },
    { frames_wizzard_m_hit, 1, 8, 14 },
    { frames_wizzard_m_idle, 4, 8, 14 },
    { frames_wizzard_m_run, 4, 8, 14 },
    { frames_dwarf_m_idle, 4, 8, 14 },
    { frames_imp_idle, 4, 8, 8 },
    { frames_imp_run, 4, 8, 8 },
    { frames_lizard_f_hit, 1, 8, 14 },
    { frames_lizard_f_idle, 4, 8, 14 },
    { frames_lizard_f_run, 4, 8, 14 },
    { frames_dwarf_m_run, 4, 8, 14 },
    { frames_dwarf_m_hit, 1, 8, 14 },
    { frames_knight_f_hit, 1, 8, 14 },
    { frames_knight_f_idle, 4, 8, 14 },
    { frames_knight_f_run, 4, 8, 14 },
    { frames_dwarf_f_idle, 4, 8, 14 },
    { frames_dwarf_f_run, 4, 8, 14 },
    { frames_dwarf_f_hit, 1, 8, 14 },
    { frames_weapon_anime_sword, 1, 6, 15 },
    { frames_weapon_arrow, 1, 3, 10 },
    { frames_weapon_baton_with_spikes, 1, 5, 11 },
    { frames_weapon_big_hammer, 1, 5, 18 },
    { frames_weapon_bow, 1, 7, 13 },
    { frames_weapon_bow_2, 1, 7, 13 },
    { frames_weapon_double_axe, 1, 8, 12 },
    { frames_weapon_cleaver, 1, 4, 9 },
    { frames_weapon_duel_sword, 1, 4, 15 },
    { frames_weapon_golden_sword, 1, 5, 11 },
    { frames_weapon_green_magic_staff, 1, 4, 15 },
    { frames_weapon_hammer, 1, 5, 12 },
    { frames_weapon_katana, 1, 3, 14 },
    { frames_weapon_knife, 1, 3, 6 },
    { frames_weapon_knight_sword, 1, 5, 14 },
    { frames_weapon_lavish_sword, 1, 5, 15 },
    { frames_weapon_mace, 1, 5, 12 },
    { frames_weapon_saw_sword, 1, 5, 12 },
    { frames_weapon_machete, 1, 2, 11 },
    { frames_weapon_red_gem_sword, 1, 5, 10 },
    { frames_weapon_red_magic_staff, 1, 4, 15 },
    { frames_weapon_regular_sword, 1, 5, 10 },
    { frames_weapon_rusty_sword, 1, 5, 10 },
    { frames_weapon_spear, 1, 3, 15 },
    { frames_weapon_waraxe, 1, 6, 11 },
    { frames_weapon_throwing_axe, 1, 5, 7 },
    { frames_weapon_axe, 1, 4, 10 },
    { frames_wogol_idle, 4, 8, 11 },
    { frames_wogol_run, 4, 8, 11 },
    { frames_zombie, 4, 8, 8 },
    { frames_tiny_zombie_idle, 4, 8, 8 },
    { frames_tiny_zombie_run, 4, 8, 8 },
    { frames_goblin_idle, 4, 8, 8 },
    { frames_goblin_run, 4, 8, 8 },
    { frames_ice_zombie, 4, 8, 8 },
    { frames_knight_m_idle, 4, 8, 14 },
    { frames_knight_m_run, 4, 8, 14 },
    { frames_knight_m_hit, 1, 8, 14 },
    { frames_crate, 1, 8, 12 },
    { frames_edge_down, 1, 8, 8 },
    { frames_orc_shaman_idle, 4, 8, 11 },
    { frames_orc_shaman_run, 4, 8, 11 },
    { frames_swampy, 4, 8, 8 },
    { frames_muddy, 4, 8, 8 },
    { frames_necromancer, 4, 8, 11 },
    { frames_masked_orc_idle, 4, 8, 11 },
    { frames_masked_orc_run, 4, 8, 11 },
    { frames_orc_warrior_idle, 4, 8, 11 },
    { frames_orc_warrior_run, 4, 8, 11 },
    { frames_skelet_idle, 4, 8, 8 },
    { frames_skelet_run, 4, 8, 8 },
    { frames_skull, 1, 8, 8 },
    { frames_ogre_idle, 4, 16, 18 },
    { frames_ogre_run, 4, 16, 18 },
    { frames_ui_heart_empty, 1, 6, 6 },
    { frames_ui_heart_full, 1, 6, 6 },
    { frames_ui_heart_half, 1, 6, 6 },
    { frames_doc_idle, 4, 8, 11 },
    { frames_doc_run, 4, 8, 11 },
    { frames_pumpkin_dude_idle, 4, 8, 11 },
    { frames_pumpkin_dude_run, 4, 8, 11 },
    { frames_angel_idle, 4, 8, 8 },
    { frames_angel_run, 4, 8, 8 },
    { frames_chort_idle, 4, 8, 11 },
    { frames_chort_run, 4, 8, 11 },
    { frames_column, 1, 8, 24 },
    { frames_column_wall, 1, 8, 24 },
    { frames_wall_fountain_mid_blue, 3, 8, 8 },
    { frames_wall_fountain_mid_red, 3, 8, 8 },
    { frames_wall_fountain_basin_red, 3, 8, 8 },
    { frames_wall_fountain_basin_blue, 3, 8, 8 },
    { frames_wall_goo_base, 1, 8, 8 },
    { frames_wall_goo, 1, 8, 8 },
    { frames_wall_hole_1, 1, 8, 8 },
    { frames_wall_hole_2, 1, 8, 8 },
    { frames_wall_banner_blue, 1, 8, 8 },
    { frames_wall_banner_red, 1, 8, 8 },
    { frames_wall_banner_green, 1, 8, 8 },
    { frames_wall_banner_yellow, 1, 8, 8 },
    { frames_floor_ladder, 1, 8, 8 },
    { frames_floor_spikes, 4, 8, 8 },
    { frames_hole, 1, 8, 8 },
    { frames_floor_stairs, 1, 8, 8 },
    { frames_floor_1, 1, 8, 8 },
    { frames_floor_2, 1, 8, 8 },
    { frames_floor_3, 1, 8, 8 },
    { frames_floor_4, 1, 8, 8 },
    { frames_floor_5, 1, 8, 8 },
    { frames_floor_6, 1, 8, 8 },
    { frames_floor_7, 1, 8, 8 },
    { frames_floor_8, 1, 8, 8 },
    { frames_wall_left, 1, 8, 8 },
    { frames_wall_mid, 1, 8, 8 },
    { frames_wall_right, 1, 8, 8 },
    { frames_wall_top_left, 1, 8, 8 },
    { frames_wall_top_mid, 1, 8, 8 },
    { frames_wall_top_right, 1, 8, 8 },
    { frames_lizard_m_idle, 4, 8, 14 },
    { frames_lizard_m_run, 4, 8, 14 },
    { frames_lizard_m_hit, 1, 8, 14 },
    { frames_elf_f_idle, 4, 8, 14 },
    { frames_elf_f_run, 4, 8, 14 },
    { frames_elf_f_hit, 1, 8, 14 },
    { frames_elf_m_idle, 4, 8, 14 },
    { frames_elf_m_run, 4, 8, 14 },
    { frames_elf_m_hit, 1, 8, 14 },
    { frames_button_red_up, 1, 8, 8 },
    { frames_button_red_down, 1, 8, 8 },
    { frames_button_blue_up, 1, 8, 8 },
    { frames_button_blue_down, 1, 8, 8 },
    { frames_lever_left, 1, 8, 8 },
    { frames_lever_right, 1, 8, 8 },
    { frames_doors_frame_left, 1, 8, 16 },
    { frames_doors_frame_right, 1, 8, 16 },
    { frames_doors_frame_top, 1, 16, 8 },
    { frames_doors_leaf_closed, 1, 16, 16 },
    { frames_doors_leaf_open, 1, 16, 16 },
    { frames_wall_edge_bottom_left, 1, 8, 8 },
    { frames_wall_edge_bottom_right, 1, 8, 8 },
    { frames_wall_edge_mid_left, 1, 8, 8 },
    { frames_wall_edge_top_left, 1, 8, 8 },
    { frames_wall_edge_left, 1, 8, 8 },
    { frames_wall_edge_top_right, 1, 8, 8 },
    { frames_wall_edge_right, 1, 8, 8 },
    { frames_wall_edge_mid_right, 1, 8, 8 },
    { frames_wall_edge_tshape_bottom_right, 1, 8, 8 },
    { frames_wall_edge_tshape_bottom_left, 1, 8, 8 },
    { frames_wall_edge_tshape_right, 1, 8, 8 },
    { frames_wall_edge_tshape_left, 1, 8, 8 },
    { frames_wall_outer_front_right, 1, 8, 8 },
    { frames_wall_outer_front_left, 1, 8, 8 },
    { frames_wall_outer_mid_left, 1, 8, 8 },
    { frames_wall_outer_top_left, 1, 8, 8 },
    { frames_wall_outer_top_right, 1, 8, 8 },
    { frames_wall_outer_mid_right, 1, 8, 8 },
    { frames_wall_fountain_top_1, 1, 8, 8 },
    { frames_wall_fountain_top_2, 1, 8, 8 },
    { frames_wall_fountain_top_3, 1, 8, 8 },
    { frames_slug, 4, 8, 11 },
    { frames_tiny_slug, 4, 8, 8 },
};
