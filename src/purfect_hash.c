#include <aion.h>

extern s32 main(void);

// clang-format off
static c8 keys[][27] = { 
  "Abha"              , "Abidjan"          , "Abéché"                    , "Accra"          , "Addis Ababa"  ,
  "Adelaide"          , "Aden"             , "Ahvaz"                     , "Albuquerque"    , "Alexandra"    ,
  "Alexandria"        , "Algiers"          , "Alice Springs"             , "Almaty"         , "Amsterdam"    ,
  "Anadyr"            , "Anchorage"        , "Andorra la Vella"          , "Ankara"         , "Antananarivo" ,
  "Antsiranana"       , "Arkhangelsk"      , "Ashgabat"                  , "Asmara"         , "Assab"        ,
  "Astana"            , "Athens"           , "Atlanta"                   , "Auckland"       , "Austin"       ,
  "Baghdad"           , "Baguio"           , "Baku"                      , "Baltimore"      , "Bamako"       ,
  "Bangkok"           , "Bangui"           , "Banjul"                    , "Barcelona"      , "Bata"         ,
  "Batumi"            , "Beijing"          , "Beirut"                    , "Belgrade"       , "Belize City"  ,
  "Benghazi"          , "Bergen"           , "Berlin"                    , "Bilbao"         , "Birao"        ,
  "Bishkek"           , "Bissau"           , "Blantyre"                  , "Bloemfontein"   , "Boise"        ,
  "Bordeaux"          , "Bosaso"           , "Boston"                    , "Bouaké"         , "Bratislava"   ,
  "Brazzaville"       , "Bridgetown"       , "Brisbane"                  , "Brussels"       , "Bucharest"    ,
  "Budapest"          , "Bujumbura"        , "Bulawayo"                  , "Burnie"         , "Busan"        ,
  "Cabo San Lucas"    , "Cairns"           , "Cairo"                     , "Calgary"        , "Canberra"     ,
  "Cape Town"         , "Changsha"         , "Charlotte"                 , "Chiang Mai"     , "Chicago"      ,
  "Chihuahua"         , "Chișinău"         , "Chittagong"                , "Chongqing"      , "Christchurch" ,
  "City of San Marino", "Colombo"          , "Columbus"                  , "Conakry"        , "Copenhagen"   ,
  "Cotonou"           , "Cracow"           , "Da Lat"                    , "Da Nang"        , "Dakar"        ,
  "Dallas"            , "Damascus"         , "Dampier"                   , "Dar es Salaam"  , "Darwin"       ,
  "Denpasar"          , "Denver"           , "Detroit"                   , "Dhaka"          , "Dikson"       ,
  "Dili"              , "Djibouti"         , "Dodoma"                    , "Dolisie"        , "Douala"       ,
  "Dubai"             , "Dublin"           , "Dunedin"                   , "Durban"         , "Dushanbe"     ,
  "Edinburgh"         , "Edmonton"         , "El Paso"                   , "Entebbe"        , "Erbil"        ,
  "Erzurum"           , "Fairbanks"        , "Fianarantsoa"              , "Flores"         , "Frankfurt"    ,
  "Fresno"            , "Fukuoka"          , "Gabès"                     , "Gaborone"       , "Gagnoa"       ,
  "Gangtok"           , "Garissa"          , "Garoua"                    , "George Town"    , "Ghanzi"       ,
  "Gjoa Haven"        , "Guadalajara"      , "Guangzhou"                 , "Guatemala City" , "Halifax"      ,
  "Hamburg"           , "Hamilton"         , "Hanga Roa"                 , "Hanoi"          , "Harare"       ,
  "Harbin"            , "Hargeisa"         , "Hat Yai"                   , "Havana"         , "Helsinki"     ,
  "Heraklion"         , "Hiroshima"        , "Ho Chi Minh City"          , "Hobart"         , "Hong Kong"    ,
  "Honiara"           , "Honolulu"         , "Houston"                   , "Ifrane"         , "Indianapolis" ,
  "Iqaluit"           , "Irkutsk"          , "Istanbul"                  , "İzmir"          , "Jacksonville" ,
  "Jakarta"           , "Jayapura"         , "Jerusalem"                 , "Johannesburg"   , "Jos"          ,
  "Juba"              , "Kabul"            , "Kampala"                   , "Kandi"          , "Kankan"       ,
  "Kano"              , "Kansas City"      , "Karachi"                   , "Karonga"        , "Kathmandu"    ,
  "Khartoum"          , "Kingston"         , "Kinshasa"                  , "Kolkata"        , "Kuala Lumpur" ,
  "Kumasi"            , "Kunming"          , "Kuopio"                    , "Kuwait City"    , "Kyiv"         ,
  "Kyoto"             , "La Ceiba"         , "La Paz"                    , "Lagos"          , "Lahore"       ,
  "Lake Havasu City"  , "Lake Tekapo"      , "Las Palmas de Gran Canaria", "Las Vegas"      , "Launceston"   ,
  "Lhasa"             , "Libreville"       , "Lisbon"                    , "Livingstone"    , "Ljubljana"    ,
  "Lodwar"            , "Lomé"             , "London"                    , "Los Angeles"    , "Louisville"   ,
  "Luanda"            , "Lubumbashi"       , "Lusaka"                    , "Luxembourg City", "Lviv"         ,
  "Lyon"              , "Madrid"           , "Mahajanga"                 , "Makassar"       , "Makurdi"      ,
  "Malabo"            , "Malé"             , "Managua"                   , "Manama"         , "Mandalay"     ,
  "Mango"             , "Manila"           , "Maputo"                    , "Marrakesh"      , "Marseille"    ,
  "Maun"              , "Medan"            , "Mek'ele"                   , "Melbourne"      , "Memphis"      ,
  "Mexicali"          , "Mexico City"      , "Miami"                     , "Milan"          , "Milwaukee"    ,
  "Minneapolis"       , "Minsk"            , "Mogadishu"                 , "Mombasa"        , "Monaco"       ,
  "Moncton"           , "Monterrey"        , "Montreal"                  , "Moscow"         , "Mumbai"       ,
  "Murmansk"          , "Muscat"           , "Mzuzu"                     , "N'Djamena"      , "Naha"         ,
  "Nairobi"           , "Nakhon Ratchasima", "Napier"                    , "Napoli"         , "Nashville"    ,
  "Nassau"            , "Ndola"            , "New Delhi"                 , "New Orleans"    , "New York City",
  "Ngaoundéré"        , "Niamey"           , "Nicosia"                   , "Niigata"        , "Nouadhibou"   ,
  "Nouakchott"        , "Novosibirsk"      , "Nuuk"                      , "Odesa"          , "Odienné"      ,
  "Oklahoma City"     , "Omaha"            , "Oranjestad"                , "Oslo"           , "Ottawa"       ,
  "Ouagadougou"       , "Ouahigouya"       , "Ouarzazate"                , "Oulu"           , "Palembang"    ,
  "Palermo"           , "Palm Springs"     , "Palmerston North"          , "Panama City"    , "Parakou"      ,
  "Paris"             , "Perth"            , "Petropavlovsk-Kamchatsky"  , "Philadelphia"   , "Phnom Penh"   ,
  "Phoenix"           , "Pittsburgh"       , "Podgorica"                 , "Pointe-Noire"   , "Pontianak"    ,
  "Port Moresby"      , "Port Sudan"       , "Port Vila"                 , "Port-Gentil"    , "Portland (OR)",
  "Porto"             , "Prague"           , "Praia"                     , "Pretoria"       , "Pyongyang"    ,
  "Rabat"             , "Rangpur"          , "Reggane"                   , "Reykjavík"      , "Riga"         ,
  "Riyadh"            , "Rome"             , "Roseau"                    , "Rostov-on-Don"  , "Sacramento"   ,
  "Saint Petersburg"  , "Saint-Pierre"     , "Salt Lake City"            , "San Antonio"    , "San Diego"    ,
  "San Francisco"     , "San Jose"         , "San José"                  , "San Juan"       , "San Salvador" ,
  "Sana'a"            , "Santo Domingo"    , "Sapporo"                   , "Sarajevo"       , "Saskatoon"    ,
  "Seattle"           , "Ségou"            , "Seoul"                     , "Seville"        , "Shanghai"     ,
  "Singapore"         , "Skopje"           , "Sochi"                     , "Sofia"          , "Sokoto"       ,
  "Split"             , "St. John's"       , "St. Louis"                 , "Stockholm"      , "Surabaya"     ,
  "Suva"              , "Suwałki"          , "Sydney"                    , "Tabora"         , "Tabriz"       ,
  "Taipei"            , "Tallinn"          , "Tamale"                    , "Tamanrasset"    , "Tampa"        ,
  "Tashkent"          , "Tauranga"         , "Tbilisi"                   , "Tegucigalpa"    , "Tehran"       ,
  "Tel Aviv"          , "Thessaloniki"     , "Thiès"                     , "Tijuana"        , "Timbuktu"     ,
  "Tirana"            , "Toamasina"        , "Tokyo"                     , "Toliara"        , "Toluca"       ,
  "Toronto"           , "Tripoli"          , "Tromsø"                    , "Tucson"         , "Tunis"        ,
  "Ulaanbaatar"       , "Upington"         , "Ürümqi"                    , "Vaduz"          , "Valencia"     ,
  "Valletta"          , "Vancouver"        , "Veracruz"                  , "Vienna"         , "Vientiane"    ,
  "Villahermosa"      , "Vilnius"          , "Virginia Beach"            , "Vladivostok"    , "Warsaw"       ,
  "Washington"        , "Wau"              , "Wellington"                , "Whitehorse"     , "Wichita"      ,
  "Willemstad"        , "Winnipeg"         , "Wrocław"                   , "Xi'an"          , "Yakutsk"      ,
  "Yangon"            , "Yaoundé"          , "Yellowknife"               , "Yerevan"        , "Yinchuan"     ,
  "Zagreb"            , "Zanzibar City"    , "Zürich"
};
// clang-format on

static s32 G[] = {
    3,    50,   84,   27,   21,    42,    12,   6,    46,   444, 46,   8,
    215,  185,  1,    172,  231,   4,     485,  220,  81,   1,   1430, 1981,
    7,    391,  156,  66,   1,     20,    1836, 8,    904,  342, 3,    17,
    153,  0,    5,    811,  72,    322,   5489, 1,    2408, 363, 106,  224,
    1583, 2066, 219,  330,  1097,  2,     54,   222,  83,   3,   3422, 5027,
    -409, 17,   124,  2,    167,   5,     684,  445,  49,   2,   68,   36,
    2383, 124,  4460, 3827, 35945, 18217, 350,  5438, 9,    -255};
static s32 V[] = {
    20,  188, 290, 153, 214, 307, 213, 164, 133, 170, 384, 8,   275, 412, 376,
    144, 374, 152, 163, 378, 167, 174, 69,  104, 250, 132, 156, 228, 227, 161,
    272, 43,  35,  148, 266, 21,  186, 204, 169, 398, 379, 295, 70,  33,  338,
    121, 146, 222, 191, 25,  92,  256, 306, 71,  125, 187, 2,   86,  238, 73,
    333, 109, 255, 147, 351, 411, 359, 59,  36,  61,  365, 58,  141, 68,  75,
    39,  159, 127, 171, 212, 237, 262, 87,  312, 299, 179, 313, 258, 265, 130,
    336, 389, 57,  209, 17,  6,   135, 318, 194, 249, 113, 381, 357, 341, 352,
    310, 119, 268, 248, 353, 85,  327, 180, 347, 65,  305, 407, 332, 122, 358,
    225, 79,  215, 218, 60,  67,  232, 44,  235, 288, 1,   393, 322, 402, 183,
    143, 80,  344, 15,  200, 56,  142, 410, 166, 22,  202, 311, 294, 278, 226,
    11,  298, 289, 10,  189, 243, 282, 48,  240, 19,  29,  116, 124, 102, 251,
    342, 114, 195, 264, 192, 150, 406, 217, 120, 38,  108, 196, 287, 211, 134,
    172, 198, 276, 345, 392, 42,  175, 321, 31,  367, 269, 184, 62,  84,  128,
    259, 220, 112, 390, 320, 205, 346, 105, 360, 317, 158, 149, 34,  208, 377,
    76,  297, 88,  231, 301, 385, 77,  401, 182, 361, 197, 383, 193, 223, 5,
    89,  362, 337, 366, 177, 373, 78,  53,  96,  199, 280, 72,  283, 386, 41,
    9,   107, 91,  0,   37,  168, 110, 101, 263, 139, 304, 335, 4,   97,  26,
    106, 396, 326, 94,  138, 12,  45,  303, 117, 349, 154, 369, 356, 354, 16,
    409, 165, 296, 302, 52,  30,  185, 151, 329, 129, 397, 230, 253, 364, 339,
    201, 375, 63,  181, 404, 100, 309, 245, 400, 387, 372, 82,  271, 207, 325,
    371, 236, 83,  270, 246, 277, 14,  81,  281, 136, 274, 3,   64,  395, 137,
    229, 157, 388, 247, 140, 210, 176, 54,  355, 370, 47,  291, 285, 74,  51,
    241, 160, 394, 315, 233, 350, 330, 219, 239, 403, 254, 340, 273, 173, 178,
    155, 368, 27,  408, 413, 224, 50,  115, 267, 7,   308, 334, 292, 99,  98,
    103, 18,  257, 221, 405, 32,  399, 145, 331, 382, 95,  13,  316, 46,  234,
    203, 300, 391, 314, 126, 252, 90,  260, 55,  286, 363, 328, 93,  66,  28,
    190, 216, 324, 261, 123, 206, 343, 319, 242, 293, 284, 279, 162, 131, 23,
    380, 118, 323, 24,  49,  111, 244, 348};

static s64 hash(s64 d, c8 *chars) {
  if (d == 0) {
    d = 0x811C9DC5;
  }
  u8 ch;
  while ((ch = *chars++) != '\0') {
    d = d ^ ch * 16777619 & 0xffffffff;
  }
  return d;
}

s32 main(void) {
  bool used[413] = {0};
  memset(used, 0, sizeof(used));
  bool found = true;
  for (u32 i = 0; i != COUNT(keys); ++i) {
    s32 index;
    s64 h = hash(0, keys[i]);
    s32 d = G[h % COUNT(G)];
    if (d == 0) {
      index = 39;
    } else {
      if (d == 0) {
        d = h % COUNT(V);
      } else if (d < 0) {
        d = -d - 1;
      } else {
        d = hash(d, keys[i]) % COUNT(V);
      }
      index = V[d] - 1;
    }

    if (used[index]) {
      found = false;
    }
    used[index] = true;
  }
  // printf("COUNT(G)=%lu\n", COUNT(G));
  // printf("COUNT(V)=%lu\n", COUNT(V));
  return found ? 0 : 1;
}
