#include <aion.h>
#include <stdlib.h>

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
  "Erzurum"           , "Fairbanks"        , "Fianarantsoa"              , "Flores,  Petén" , "Frankfurt"    ,
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
  "Washington, D.C."  , "Wau"              , "Wellington"                , "Whitehorse"     , "Wichita"      ,
  "Willemstad"        , "Winnipeg"         , "Wrocław"                   , "Xi'an"          , "Yakutsk"      ,
  "Yangon"            , "Yaoundé"          , "Yellowknife"               , "Yerevan"        , "Yinchuan"     ,
  "Zagreb"            , "Zanzibar City"    , "Zürich"
};
// clang-format on

static s32 G[] = {9,    4,  6,  14,   4,  4,    1,  0,  12, 3,  37, 1,    20,
                  1,    1,  10, -521, 4,  -517, 1,  25, 8,  5,  2,  12,   4,
                  5,    3,  1,  4,    1,  1,    12, 1,  16, 4,  5,  24,   8,
                  5,    16, 4,  3,    1,  1,    1,  4,  6,  24, 8,  -514, -498,
                  10,   24, 23, 2,    1,  86,   14, 11, 8,  19, 20, 5,    1,
                  -497, 68, 30, 12,   34, 7,    10, 11, 95, 2,  3,  16,   1,
                  5,    6,  28, 31,   20, 1,    44, 77, 20, 4,  55, 1,    3,
                  -493, 1,  3,  4,    1,  1,    6,  38, 4,  10, 5,  15,   5};
#define None -1
static s32 V[] = {
    318,  None, 52,   311,  61,   335,  280,  104,  182,  123,  None, None,
    251,  404,  279,  4,    299,  None, None, 366,  386,  281,  98,   13,
    333,  356,  None, 313,  None, 242,  348,  47,   410,  150,  None, None,
    None, 77,   None, None, 79,   None, None, None, None, 240,  None, 358,
    315,  None, 355,  290,  None, None, 344,  393,  212,  None, 379,  None,
    70,   302,  None, 168,  211,  174,  None, 46,   None, 118,  328,  None,
    None, None, None, 62,   359,  63,   None, 233,  259,  263,  None, None,
    238,  25,   285,  347,  141,  42,   138,  None, 36,   None, 183,  10,
    192,  38,   262,  None, 268,  None, 218,  None, 413,  None, 94,   330,
    None, 243,  None, None, 194,  249,  None, 245,  None, 308,  252,  2,
    102,  None, None, 17,   55,   272,  334,  40,   199,  327,  93,   140,
    362,  134,  224,  244,  None, 387,  351,  374,  None, 203,  361,  282,
    20,   9,    None, 369,  205,  None, 345,  346,  None, 198,  None, 86,
    164,  None, 319,  143,  None, 254,  68,   None, 101,  181,  87,   None,
    173,  402,  405,  400,  45,   30,   None, 352,  301,  229,  132,  228,
    316,  None, 323,  373,  None, 96,   380,  201,  121,  275,  None, 162,
    220,  92,   155,  148,  230,  None, 142,  208,  264,  283,  332,  392,
    190,  231,  56,   None, 371,  343,  326,  67,   412,  139,  95,   391,
    397,  136,  33,   185,  411,  157,  176,  7,    None, 180,  329,  260,
    401,  110,  377,  388,  365,  None, 265,  None, 219,  35,   286,  None,
    135,  None, 172,  None, None, None, 214,  89,   None, 193,  288,  None,
    312,  298,  64,   None, 406,  137,  16,   363,  145,  287,  21,   109,
    39,   41,   156,  317,  None, 81,   292,  31,   32,   151,  322,  113,
    271,  146,  171,  277,  76,   3,    28,   14,   284,  115,  278,  340,
    None, 267,  305,  None, 29,   51,   12,   None, 232,  None, 8,    241,
    202,  169,  186,  223,  None, 196,  257,  None, 152,  None, None, 324,
    147,  None, 60,   88,   269,  None, 372,  166,  225,  34,   163,  153,
    314,  384,  398,  117,  296,  49,   None, 378,  354,  18,   222,  294,
    170,  235,  382,  310,  None, 108,  53,   353,  None, None, None, 197,
    408,  23,   175,  144,  None, 114,  177,  165,  357,  None, 253,  129,
    99,   395,  399,  273,  75,   221,  250,  200,  161,  122,  11,   367,
    131,  73,   320,  210,  57,   19,   26,   None, 306,  188,  394,  None,
    None, None, 1,    125,  44,   126,  381,  159,  None, 207,  111,  127,
    82,   None, 187,  274,  375,  209,  403,  189,  293,  58,   370,  179,
    None, 304,  59,   215,  103,  80,   338,  130,  119,  216,  309,  158,
    120,  239,  90,   128,  None, 297,  331,  289,  390,  112,  160,  364,
    None, 237,  261,  276,  None, 336,  385,  295,  24,   66,   325,  84,
    270,  106,  226,  None, 206,  234,  None, 184,  85,   78,   227,  27,
    None, 178,  37,   149,  376,  74,   5,    None, 407,  191,  100,  71,
    6,    72,   124,  247,  339,  None, 303,  None, None, 300,  43,   258,
    154,  50,   48,   15,   None, 342,  349,  321,  167,  22,   337,  None,
    266,  105,  91,   195,  256,  83,   69,   236,  255,  307,  204,  54,
    396,  409,  360,  107,  217,  389,  116,  350,  213,  368,  248,  133,
    291,  383,  65,   341,  97};

static bool log = false;
static s64 hash(s64 d, c8 *chars) {
  if (d == 0) {
    d = 0x811C9DC5;
  }
  u8 ch;
  while ((ch = *chars++) != '\0') {
    d = d ^ ch * 16777619 & 0xffffffff;
    if (log) {
      printf("%08lx %hhu\n", d, ch & 0xff);
    }
  }
  return d;
}

struct station {
  f32 min, max, sum;
  u64 count;
};

void process(c8 *begin, c8 *end, struct station *s) {
  c8 ch;
  do {
    c8 *line_begin = begin;
    s64 h = 0x811C9DC5;
    while ((ch = *begin++) != ';') {
      s64 pp = ch & 0xff;
      h = h ^ pp * 16777619 & 0xffffffff;
    }
    c8 *line_end = begin - 1;

    // if (strncmp(line_begin, "Busan", strlen("Busan")) == 0) {
    //   {
    //     printf("\n");
    //     printf("\n");
    //     c8 *begin2 = line_begin;
    //     s64 h = 0x811C9DC5;
    //     while ((ch = *begin2++) != ';') {
    //       s64 pp = ch & 0xff;
    //       h = h ^ pp * 16777619;
    //       printf("%08lx %ld \n", h, pp);
    //     }
    //     printf("\n");
    //     s64 d = G[h % COUNT(G)];
    //     for (c8 *it = line_begin; it != line_end; ++it) {
    //       s64 pp = *it & 0xff;
    //       d = d ^ pp * 16777619 & 0xffffffff;
    //       printf("%08lx %ld \n", d, pp);
    //     }
    //     printf("\n");
    //     d = d % COUNT(V);
    //     printf("Index %d\n", V[d]);
    //     printf("\n");
    //   }
    // }

    s64 d = G[h % COUNT(G)];
    s32 index;
    if (d == 0) {
      index = 245;
    } else {
      if (d < 0) {
        d = -d - 1;
      } else {
        for (c8 *it = line_begin; it != line_end; ++it) {
          s64 pp = *it & 0xff;
          d = d ^ pp * 16777619 & 0xffffffff;
        }
        d = d % COUNT(V);
      }
      index = V[d] - 1;
    }

    ASSERT(0 <= index && index < 414);

    f32 n;
    ch = *begin++;
    f32 sign = 1;
    if (ch == '-') {
      ch = *begin++;
      ASSERT('0' <= ch && ch <= '9');
      sign = -1;
      n = ch - '0';
    } else {
      ASSERT('0' <= ch && ch <= '9');
      n = ch - '0';
    }
    while ((ch = *begin++) != '.') {
      ASSERT('0' <= ch && ch <= '9');
      n = n * 10 + (ch - '0');
    }
    ch = *begin++;
    ASSERT('0' <= ch && ch <= '9');
    n += (ch - '0') / 10.0f;
    ch = *begin++;
    ASSERT(ch == '\n');

    if (s[index].count == 0) {
      printf("%s, %.*s, %d\n", keys[index], (s32)(line_end - line_begin),
             line_begin, index);
    }
    n *= sign;
    s[index].sum += n;
    s[index].count += 1;
    if (s[index].max < n) {
      s[index].max = n;
    }
    if (s[index].min > n) {
      s[index].min = n;
    }

  } while (begin != end);
}

s32 main(void) {
  {
    u32 i = 81;
    c8 *key = keys[i];
    s32 index;
    log = true;
    s64 h = hash(0, key);
    log = false;
    s32 d = G[h % COUNT(G)];
    if (d == 0) {
      d = h % COUNT(V);
    } else if (d < 0) {
      d = -d - 1;
    } else {
      d = hash(d, key) % COUNT(V);
    }
    index = V[d] - 1;
    printf("%s = %d\n", keys[i], i);
    // return 0;
  }

  struct station s[414];
  memset(s, 0, sizeof(s));
  for (u32 i = 0; i != COUNT(s); ++i) {
    s[i].min = 500.0f;
    s[i].max = -500.0f;
  }
  u64 size = 0;
  c8 *chunk_begin;
  c8 *begin = chunk_begin =
      file_map("/home/purport/Code/datastructures/measurements.txt", &size);
  if (begin == NULL) {
    printf("Failed to open file\n");
    return 1;
  }

  u64 chunk_size = size / 8;
  printf("File size is %lu, processing in chunks of %lu\n", size, chunk_size);
  c8 *end = &begin[size];
  while (chunk_begin != end) {
    c8 *chunk_end = &chunk_begin[chunk_size];
    if (chunk_end > end || ((u64)(end - chunk_end)) < chunk_size) {
      chunk_end = end;
    } else {
      while (chunk_end[-1] != '\n') {
        --chunk_end;
      }
    }
    printf("Chunk %08lx to %08lx\n", chunk_begin - begin, chunk_end - begin);
    process(chunk_begin, chunk_end, s);
    chunk_begin = chunk_end;
  }

  u64 count = 0;
  for (u32 i = 0; i != COUNT(s); ++i) {
    struct station st = s[i];
    count += st.count;
    if (st.count == 0) {
      continue;
    }
    printf("%s=%.1f/%.1f/%.1f, ", keys[i], st.min, st.sum / st.count, st.max);
  }

  printf("\n\n%lu lines in file\n", count);
}
