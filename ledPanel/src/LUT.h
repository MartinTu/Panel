/*
 * LUT.h
 *
 *  Created on: 12.08.2015
 *      Author: keks
 */

#ifndef LUT_H_
#define LUT_H_

#define SINSAMPLECOUNT 1024
static const float sin_samples[1024] = {
 0.0000000000,  0.0061358846,  0.0122715383,  0.0184067299,  0.0245412285,  0.0306748032,  0.0368072229,  0.0429382569,
 0.0490676743,  0.0551952443,  0.0613207363,  0.0674439196,  0.0735645636,  0.0796824380,  0.0857973123,  0.0919089565,
 0.0980171403,  0.1041216339,  0.1102222073,  0.1163186309,  0.1224106752,  0.1284981108,  0.1345807085,  0.1406582393,
 0.1467304745,  0.1527971853,  0.1588581433,  0.1649131205,  0.1709618888,  0.1770042204,  0.1830398880,  0.1890686641,
 0.1950903220,  0.2011046348,  0.2071113762,  0.2131103199,  0.2191012402,  0.2250839114,  0.2310581083,  0.2370236060,
 0.2429801799,  0.2489276057,  0.2548656596,  0.2607941179,  0.2667127575,  0.2726213554,  0.2785196894,  0.2844075372,
 0.2902846773,  0.2961508882,  0.3020059493,  0.3078496400,  0.3136817404,  0.3195020308,  0.3253102922,  0.3311063058,
 0.3368898534,  0.3426607173,  0.3484186802,  0.3541635254,  0.3598950365,  0.3656129978,  0.3713171940,  0.3770074102,
 0.3826834324,  0.3883450467,  0.3939920401,  0.3996241998,  0.4052413140,  0.4108431711,  0.4164295601,  0.4220002708,
 0.4275550934,  0.4330938189,  0.4386162385,  0.4441221446,  0.4496113297,  0.4550835871,  0.4605387110,  0.4659764958,
 0.4713967368,  0.4767992301,  0.4821837721,  0.4875501601,  0.4928981922,  0.4982276670,  0.5035383837,  0.5088301425,
 0.5141027442,  0.5193559902,  0.5245896827,  0.5298036247,  0.5349976199,  0.5401714727,  0.5453249884,  0.5504579729,
 0.5555702330,  0.5606615762,  0.5657318108,  0.5707807459,  0.5758081914,  0.5808139581,  0.5857978575,  0.5907597019,
 0.5956993045,  0.6006164794,  0.6055110414,  0.6103828063,  0.6152315906,  0.6200572118,  0.6248594881,  0.6296382389,
 0.6343932842,  0.6391244449,  0.6438315429,  0.6485144010,  0.6531728430,  0.6578066933,  0.6624157776,  0.6669999223,
 0.6715589548,  0.6760927036,  0.6806009978,  0.6850836678,  0.6895405447,  0.6939714609,  0.6983762494,  0.7027547445,
 0.7071067812,  0.7114321957,  0.7157308253,  0.7200025080,  0.7242470830,  0.7284643904,  0.7326542717,  0.7368165689,
 0.7409511254,  0.7450577854,  0.7491363945,  0.7531867990,  0.7572088465,  0.7612023855,  0.7651672656,  0.7691033376,
 0.7730104534,  0.7768884657,  0.7807372286,  0.7845565972,  0.7883464276,  0.7921065773,  0.7958369046,  0.7995372691,
 0.8032075315,  0.8068475535,  0.8104571983,  0.8140363297,  0.8175848132,  0.8211025150,  0.8245893028,  0.8280450453,
 0.8314696123,  0.8348628750,  0.8382247056,  0.8415549774,  0.8448535652,  0.8481203448,  0.8513551931,  0.8545579884,
 0.8577286100,  0.8608669386,  0.8639728561,  0.8670462455,  0.8700869911,  0.8730949784,  0.8760700942,  0.8790122264,
 0.8819212643,  0.8847970984,  0.8876396204,  0.8904487232,  0.8932243012,  0.8959662498,  0.8986744657,  0.9013488470,
 0.9039892931,  0.9065957045,  0.9091679831,  0.9117060320,  0.9142097557,  0.9166790599,  0.9191138517,  0.9215140393,
 0.9238795325,  0.9262102421,  0.9285060805,  0.9307669611,  0.9329927988,  0.9351835099,  0.9373390119,  0.9394592236,
 0.9415440652,  0.9435934582,  0.9456073254,  0.9475855910,  0.9495281806,  0.9514350210,  0.9533060404,  0.9551411683,
 0.9569403357,  0.9587034749,  0.9604305194,  0.9621214043,  0.9637760658,  0.9653944417,  0.9669764710,  0.9685220943,
 0.9700312532,  0.9715038910,  0.9729399522,  0.9743393828,  0.9757021300,  0.9770281427,  0.9783173707,  0.9795697657,
 0.9807852804,  0.9819638691,  0.9831054874,  0.9842100924,  0.9852776424,  0.9863080972,  0.9873014182,  0.9882575677,
 0.9891765100,  0.9900582103,  0.9909026354,  0.9917097537,  0.9924795346,  0.9932119492,  0.9939069700,  0.9945645707,
 0.9951847267,  0.9957674145,  0.9963126122,  0.9968202993,  0.9972904567,  0.9977230666,  0.9981181129,  0.9984755806,
 0.9987954562,  0.9990777278,  0.9993223846,  0.9995294175,  0.9996988187,  0.9998305818,  0.9999247018,  0.9999811753,
 1.0000000000,  0.9999811753,  0.9999247018,  0.9998305818,  0.9996988187,  0.9995294175,  0.9993223846,  0.9990777278,
 0.9987954562,  0.9984755806,  0.9981181129,  0.9977230666,  0.9972904567,  0.9968202993,  0.9963126122,  0.9957674145,
 0.9951847267,  0.9945645707,  0.9939069700,  0.9932119492,  0.9924795346,  0.9917097537,  0.9909026354,  0.9900582103,
 0.9891765100,  0.9882575677,  0.9873014182,  0.9863080972,  0.9852776424,  0.9842100924,  0.9831054874,  0.9819638691,
 0.9807852804,  0.9795697657,  0.9783173707,  0.9770281427,  0.9757021300,  0.9743393828,  0.9729399522,  0.9715038910,
 0.9700312532,  0.9685220943,  0.9669764710,  0.9653944417,  0.9637760658,  0.9621214043,  0.9604305194,  0.9587034749,
 0.9569403357,  0.9551411683,  0.9533060404,  0.9514350210,  0.9495281806,  0.9475855910,  0.9456073254,  0.9435934582,
 0.9415440652,  0.9394592236,  0.9373390119,  0.9351835099,  0.9329927988,  0.9307669611,  0.9285060805,  0.9262102421,
 0.9238795325,  0.9215140393,  0.9191138517,  0.9166790599,  0.9142097557,  0.9117060320,  0.9091679831,  0.9065957045,
 0.9039892931,  0.9013488470,  0.8986744657,  0.8959662498,  0.8932243012,  0.8904487232,  0.8876396204,  0.8847970984,
 0.8819212643,  0.8790122264,  0.8760700942,  0.8730949784,  0.8700869911,  0.8670462455,  0.8639728561,  0.8608669386,
 0.8577286100,  0.8545579884,  0.8513551931,  0.8481203448,  0.8448535652,  0.8415549774,  0.8382247056,  0.8348628750,
 0.8314696123,  0.8280450453,  0.8245893028,  0.8211025150,  0.8175848132,  0.8140363297,  0.8104571983,  0.8068475535,
 0.8032075315,  0.7995372691,  0.7958369046,  0.7921065773,  0.7883464276,  0.7845565972,  0.7807372286,  0.7768884657,
 0.7730104534,  0.7691033376,  0.7651672656,  0.7612023855,  0.7572088465,  0.7531867990,  0.7491363945,  0.7450577854,
 0.7409511254,  0.7368165689,  0.7326542717,  0.7284643904,  0.7242470830,  0.7200025080,  0.7157308253,  0.7114321957,
 0.7071067812,  0.7027547445,  0.6983762494,  0.6939714609,  0.6895405447,  0.6850836678,  0.6806009978,  0.6760927036,
 0.6715589548,  0.6669999223,  0.6624157776,  0.6578066933,  0.6531728430,  0.6485144010,  0.6438315429,  0.6391244449,
 0.6343932842,  0.6296382389,  0.6248594881,  0.6200572118,  0.6152315906,  0.6103828063,  0.6055110414,  0.6006164794,
 0.5956993045,  0.5907597019,  0.5857978575,  0.5808139581,  0.5758081914,  0.5707807459,  0.5657318108,  0.5606615762,
 0.5555702330,  0.5504579729,  0.5453249884,  0.5401714727,  0.5349976199,  0.5298036247,  0.5245896827,  0.5193559902,
 0.5141027442,  0.5088301425,  0.5035383837,  0.4982276670,  0.4928981922,  0.4875501601,  0.4821837721,  0.4767992301,
 0.4713967368,  0.4659764958,  0.4605387110,  0.4550835871,  0.4496113297,  0.4441221446,  0.4386162385,  0.4330938189,
 0.4275550934,  0.4220002708,  0.4164295601,  0.4108431711,  0.4052413140,  0.3996241998,  0.3939920401,  0.3883450467,
 0.3826834324,  0.3770074102,  0.3713171940,  0.3656129978,  0.3598950365,  0.3541635254,  0.3484186802,  0.3426607173,
 0.3368898534,  0.3311063058,  0.3253102922,  0.3195020308,  0.3136817404,  0.3078496400,  0.3020059493,  0.2961508882,
 0.2902846773,  0.2844075372,  0.2785196894,  0.2726213554,  0.2667127575,  0.2607941179,  0.2548656596,  0.2489276057,
 0.2429801799,  0.2370236060,  0.2310581083,  0.2250839114,  0.2191012402,  0.2131103199,  0.2071113762,  0.2011046348,
 0.1950903220,  0.1890686641,  0.1830398880,  0.1770042204,  0.1709618888,  0.1649131205,  0.1588581433,  0.1527971853,
 0.1467304745,  0.1406582393,  0.1345807085,  0.1284981108,  0.1224106752,  0.1163186309,  0.1102222073,  0.1041216339,
 0.0980171403,  0.0919089565,  0.0857973123,  0.0796824380,  0.0735645636,  0.0674439196,  0.0613207363,  0.0551952443,
 0.0490676743,  0.0429382569,  0.0368072229,  0.0306748032,  0.0245412285,  0.0184067299,  0.0122715383,  0.0061358846,
 0.0000000000, -0.0061358846, -0.0122715383, -0.0184067299, -0.0245412285, -0.0306748032, -0.0368072229, -0.0429382569,
-0.0490676743, -0.0551952443, -0.0613207363, -0.0674439196, -0.0735645636, -0.0796824380, -0.0857973123, -0.0919089565,
-0.0980171403, -0.1041216339, -0.1102222073, -0.1163186309, -0.1224106752, -0.1284981108, -0.1345807085, -0.1406582393,
-0.1467304745, -0.1527971853, -0.1588581433, -0.1649131205, -0.1709618888, -0.1770042204, -0.1830398880, -0.1890686641,
-0.1950903220, -0.2011046348, -0.2071113762, -0.2131103199, -0.2191012402, -0.2250839114, -0.2310581083, -0.2370236060,
-0.2429801799, -0.2489276057, -0.2548656596, -0.2607941179, -0.2667127575, -0.2726213554, -0.2785196894, -0.2844075372,
-0.2902846773, -0.2961508882, -0.3020059493, -0.3078496400, -0.3136817404, -0.3195020308, -0.3253102922, -0.3311063058,
-0.3368898534, -0.3426607173, -0.3484186802, -0.3541635254, -0.3598950365, -0.3656129978, -0.3713171940, -0.3770074102,
-0.3826834324, -0.3883450467, -0.3939920401, -0.3996241998, -0.4052413140, -0.4108431711, -0.4164295601, -0.4220002708,
-0.4275550934, -0.4330938189, -0.4386162385, -0.4441221446, -0.4496113297, -0.4550835871, -0.4605387110, -0.4659764958,
-0.4713967368, -0.4767992301, -0.4821837721, -0.4875501601, -0.4928981922, -0.4982276670, -0.5035383837, -0.5088301425,
-0.5141027442, -0.5193559902, -0.5245896827, -0.5298036247, -0.5349976199, -0.5401714727, -0.5453249884, -0.5504579729,
-0.5555702330, -0.5606615762, -0.5657318108, -0.5707807459, -0.5758081914, -0.5808139581, -0.5857978575, -0.5907597019,
-0.5956993045, -0.6006164794, -0.6055110414, -0.6103828063, -0.6152315906, -0.6200572118, -0.6248594881, -0.6296382389,
-0.6343932842, -0.6391244449, -0.6438315429, -0.6485144010, -0.6531728430, -0.6578066933, -0.6624157776, -0.6669999223,
-0.6715589548, -0.6760927036, -0.6806009978, -0.6850836678, -0.6895405447, -0.6939714609, -0.6983762494, -0.7027547445,
-0.7071067812, -0.7114321957, -0.7157308253, -0.7200025080, -0.7242470830, -0.7284643904, -0.7326542717, -0.7368165689,
-0.7409511254, -0.7450577854, -0.7491363945, -0.7531867990, -0.7572088465, -0.7612023855, -0.7651672656, -0.7691033376,
-0.7730104534, -0.7768884657, -0.7807372286, -0.7845565972, -0.7883464276, -0.7921065773, -0.7958369046, -0.7995372691,
-0.8032075315, -0.8068475535, -0.8104571983, -0.8140363297, -0.8175848132, -0.8211025150, -0.8245893028, -0.8280450453,
-0.8314696123, -0.8348628750, -0.8382247056, -0.8415549774, -0.8448535652, -0.8481203448, -0.8513551931, -0.8545579884,
-0.8577286100, -0.8608669386, -0.8639728561, -0.8670462455, -0.8700869911, -0.8730949784, -0.8760700942, -0.8790122264,
-0.8819212643, -0.8847970984, -0.8876396204, -0.8904487232, -0.8932243012, -0.8959662498, -0.8986744657, -0.9013488470,
-0.9039892931, -0.9065957045, -0.9091679831, -0.9117060320, -0.9142097557, -0.9166790599, -0.9191138517, -0.9215140393,
-0.9238795325, -0.9262102421, -0.9285060805, -0.9307669611, -0.9329927988, -0.9351835099, -0.9373390119, -0.9394592236,
-0.9415440652, -0.9435934582, -0.9456073254, -0.9475855910, -0.9495281806, -0.9514350210, -0.9533060404, -0.9551411683,
-0.9569403357, -0.9587034749, -0.9604305194, -0.9621214043, -0.9637760658, -0.9653944417, -0.9669764710, -0.9685220943,
-0.9700312532, -0.9715038910, -0.9729399522, -0.9743393828, -0.9757021300, -0.9770281427, -0.9783173707, -0.9795697657,
-0.9807852804, -0.9819638691, -0.9831054874, -0.9842100924, -0.9852776424, -0.9863080972, -0.9873014182, -0.9882575677,
-0.9891765100, -0.9900582103, -0.9909026354, -0.9917097537, -0.9924795346, -0.9932119492, -0.9939069700, -0.9945645707,
-0.9951847267, -0.9957674145, -0.9963126122, -0.9968202993, -0.9972904567, -0.9977230666, -0.9981181129, -0.9984755806,
-0.9987954562, -0.9990777278, -0.9993223846, -0.9995294175, -0.9996988187, -0.9998305818, -0.9999247018, -0.9999811753,
-1.0000000000, -0.9999811753, -0.9999247018, -0.9998305818, -0.9996988187, -0.9995294175, -0.9993223846, -0.9990777278,
-0.9987954562, -0.9984755806, -0.9981181129, -0.9977230666, -0.9972904567, -0.9968202993, -0.9963126122, -0.9957674145,
-0.9951847267, -0.9945645707, -0.9939069700, -0.9932119492, -0.9924795346, -0.9917097537, -0.9909026354, -0.9900582103,
-0.9891765100, -0.9882575677, -0.9873014182, -0.9863080972, -0.9852776424, -0.9842100924, -0.9831054874, -0.9819638691,
-0.9807852804, -0.9795697657, -0.9783173707, -0.9770281427, -0.9757021300, -0.9743393828, -0.9729399522, -0.9715038910,
-0.9700312532, -0.9685220943, -0.9669764710, -0.9653944417, -0.9637760658, -0.9621214043, -0.9604305194, -0.9587034749,
-0.9569403357, -0.9551411683, -0.9533060404, -0.9514350210, -0.9495281806, -0.9475855910, -0.9456073254, -0.9435934582,
-0.9415440652, -0.9394592236, -0.9373390119, -0.9351835099, -0.9329927988, -0.9307669611, -0.9285060805, -0.9262102421,
-0.9238795325, -0.9215140393, -0.9191138517, -0.9166790599, -0.9142097557, -0.9117060320, -0.9091679831, -0.9065957045,
-0.9039892931, -0.9013488470, -0.8986744657, -0.8959662498, -0.8932243012, -0.8904487232, -0.8876396204, -0.8847970984,
-0.8819212643, -0.8790122264, -0.8760700942, -0.8730949784, -0.8700869911, -0.8670462455, -0.8639728561, -0.8608669386,
-0.8577286100, -0.8545579884, -0.8513551931, -0.8481203448, -0.8448535652, -0.8415549774, -0.8382247056, -0.8348628750,
-0.8314696123, -0.8280450453, -0.8245893028, -0.8211025150, -0.8175848132, -0.8140363297, -0.8104571983, -0.8068475535,
-0.8032075315, -0.7995372691, -0.7958369046, -0.7921065773, -0.7883464276, -0.7845565972, -0.7807372286, -0.7768884657,
-0.7730104534, -0.7691033376, -0.7651672656, -0.7612023855, -0.7572088465, -0.7531867990, -0.7491363945, -0.7450577854,
-0.7409511254, -0.7368165689, -0.7326542717, -0.7284643904, -0.7242470830, -0.7200025080, -0.7157308253, -0.7114321957,
-0.7071067812, -0.7027547445, -0.6983762494, -0.6939714609, -0.6895405447, -0.6850836678, -0.6806009978, -0.6760927036,
-0.6715589548, -0.6669999223, -0.6624157776, -0.6578066933, -0.6531728430, -0.6485144010, -0.6438315429, -0.6391244449,
-0.6343932842, -0.6296382389, -0.6248594881, -0.6200572118, -0.6152315906, -0.6103828063, -0.6055110414, -0.6006164794,
-0.5956993045, -0.5907597019, -0.5857978575, -0.5808139581, -0.5758081914, -0.5707807459, -0.5657318108, -0.5606615762,
-0.5555702330, -0.5504579729, -0.5453249884, -0.5401714727, -0.5349976199, -0.5298036247, -0.5245896827, -0.5193559902,
-0.5141027442, -0.5088301425, -0.5035383837, -0.4982276670, -0.4928981922, -0.4875501601, -0.4821837721, -0.4767992301,
-0.4713967368, -0.4659764958, -0.4605387110, -0.4550835871, -0.4496113297, -0.4441221446, -0.4386162385, -0.4330938189,
-0.4275550934, -0.4220002708, -0.4164295601, -0.4108431711, -0.4052413140, -0.3996241998, -0.3939920401, -0.3883450467,
-0.3826834324, -0.3770074102, -0.3713171940, -0.3656129978, -0.3598950365, -0.3541635254, -0.3484186802, -0.3426607173,
-0.3368898534, -0.3311063058, -0.3253102922, -0.3195020308, -0.3136817404, -0.3078496400, -0.3020059493, -0.2961508882,
-0.2902846773, -0.2844075372, -0.2785196894, -0.2726213554, -0.2667127575, -0.2607941179, -0.2548656596, -0.2489276057,
-0.2429801799, -0.2370236060, -0.2310581083, -0.2250839114, -0.2191012402, -0.2131103199, -0.2071113762, -0.2011046348,
-0.1950903220, -0.1890686641, -0.1830398880, -0.1770042204, -0.1709618888, -0.1649131205, -0.1588581433, -0.1527971853,
-0.1467304745, -0.1406582393, -0.1345807085, -0.1284981108, -0.1224106752, -0.1163186309, -0.1102222073, -0.1041216339,
-0.0980171403, -0.0919089565, -0.0857973123, -0.0796824380, -0.0735645636, -0.0674439196, -0.0613207363, -0.0551952443,
-0.0490676743, -0.0429382569, -0.0368072229, -0.0306748032, -0.0245412285, -0.0184067299, -0.0122715383, -0.0061358846
};

static const uint8_t gamma_samples[256] = {
	   0,       0,       0,       0,       0,       0,       0,       1,
	   1,       1,       1,       1,       1,       1,       1,       1,
	   1,       1,       2,       2,       2,       2,       2,       2,
	   2,       2,       3,       3,       3,       3,       3,       3,
	   4,       4,       4,       4,       4,       5,       5,       5,
	   5,       6,       6,       6,       6,       7,       7,       7,
	   8,       8,       8,       8,       9,       9,       9,      10,
	  10,      10,      11,      11,      11,      12,      12,      13,
	  13,      13,      14,      14,      15,      15,      16,      16,
	  16,      17,      17,      18,      18,      19,      19,      20,
	  20,      21,      21,      22,      23,      23,      24,      24,
	  25,      25,      26,      27,      27,      28,      28,      29,
	  30,      30,      31,      32,      32,      33,      34,      34,
	  35,      36,      37,      37,      38,      39,      40,      40,
	  41,      42,      43,      44,      44,      45,      46,      47,
	  48,      49,      49,      50,      51,      52,      53,      54,
	  55,      56,      57,      58,      59,      60,      61,      61,
	  62,      63,      65,      66,      67,      68,      69,      70,
	  71,      72,      73,      74,      75,      76,      77,      79,
	  80,      81,      82,      83,      84,      86,      87,      88,
	  89,      90,      92,      93,      94,      95,      97,      98,
	  99,     101,     102,     103,     105,     106,     107,     109,
	 110,     112,     113,     114,     116,     117,     119,     120,
	 122,     123,     125,     126,     128,     129,     131,     132,
	 134,     135,     137,     138,     140,     142,     143,     145,
	 147,     148,     150,     152,     153,     155,     157,     158,
	 160,     162,     164,     165,     167,     169,     171,     172,
	 174,     176,     178,     180,     182,     183,     185,     187,
	 189,     191,     193,     195,     197,     199,     201,     203,
	 205,     207,     209,     211,     213,     215,     217,     219,
	 221,     223,     225,     227,     230,     232,     234,     236,
	 238,     240,     243,     245,     247,     249,     251,     254
};

#endif