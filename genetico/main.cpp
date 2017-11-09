#include "./matingpool.h"

Genome senseiGenome() {
    // sensei 1.0
    // Genome genome(4, { 0.540986, 1.89044, -0.89429, 1.63057, 0.44571, 1.44636, -0.830971, 0.935505, -0.641196, 1.76275, 1.15792, 0.83581, -0.52066, 1.75722, -0.505592, -0.973765, -0.624946, 0.891078, 0.229185, -0.622511, 0.313494 });
    // sensei 2.0
    // Genome genome(4, { -0.314897, 1.09549, 0.0594074, 2.79274, 0.703102, -0.139028, -0.517212, 0.671523, 0.667185, 2.54945, -0.237511, 0.950038, -0.66933, 0.900584, -0.434702, -0.355428, -0.790875, 0.590007, -0.53404, 0.442229, 0.345638 });
    // sensei 3.0
    // Genome genome(4, { 0.892459, 0.667722, 0.00279838, 0.844302, 1.85919, 0.462339, 0.161734, -0.4125, -0.342346, 1.28453, 0.388194, -0.664402, 0.741212, -0.289836, -0.428056, -0.541361, 0.0209882, 0.57195, 0.948522, 0.0794782, -0.0144398 });
    // sensei suicida
    // Genome genome(4, { 0.592497, 0.0971463, -0.717764, 1.74204, -0.527168, 0.0051226, 1.3947, 0.228959, 0.644074, 0.326268, 1.52496, 0.261442, 0.916773, 0.152026, 2.20208, 0.333493, -0.36982, 0.133515, 0.410065, 1.22631, -0.384861 });
    // sensei impaciente
    //return Genome(4, { -0.0569766, 0.443566, -1.01314, 0.782724, -0.354319, 0.967402, -0.233238, -0.0337907, -1.82014, 0.308356, 0.420474, -0.277466, 0.112591, 0.344646, 0.291216, -0.895791, 0.432795, 0.630478, 0.794166, -0.456663, 0.334034 });
    // sensei ultimate 2.0
    // Genome genome(4, { 0.95848, 1.17803, -0.133761, 1.75676, 0.808261, -0.283713, -0.604071, 0.27844, 0.206845, 2.46067, 0.800298, 0.0635061, 2.63504, 0.311459, -0.938186, -0.152681, -0.139058, 0.553296, -2.47189, -0.240418, 0.551433, -0.10204, 0.781948, 0.206202 });
    // sensei ultimate 3.0 con población más randomizada
    // Genome genome(4, { 0.43636, 0.843357, 0.434914, 4.15082, -0.0461989, 0.538825, -1.16376, 0.586078, 1.15296, 2.64756, 0.982037, 0.440602, 0.731149, 0.66776, 0.0949364, -0.25537, -0.446492, 0.767926, -1.65829, -0.782782, -0.101871, -0.147309, 0.180169, 0.104889 });
    // sensei ultimate 3.1 evolution
    // return Genome(4, { 0.81168, 0.576488, 1.05681, 3.90588, 0.774202, 0.793363, -0.284316, 0.74813, -0.637652, 3.14521, 0.828814, 0.388936, 1.15498, -0.124863, -0.52746, -0.0348326, -0.565712, 1.01122, -1.94744, 0.267544, 0.790193, -0.34672, 0.120162, 0.118646 });
    return Genome(4, { -0.618926, 0.861046, -1.44393, 0.97094, 0.421278, 1.22041, 0.576201, -3.52615, 1.58669, -0.453621, -0.481932, -1.06544, 1.51374, 1.57759, 1.59665, 0.559622, -3.14645, -1.62572, -0.639372, -1.79587, -1.54029, 0.685817, 1.50501, 1.97937, -1.37573, 0.321841, 0.331086, 0.547984, 0.948096, -2.80665, -1.854, 5.55468, -1.91268, -0.621226, 0.581967, 2.21397, 1.69321, 2.43753, -1.84857, -0.901402, 0.189463, -2.52211, 0.282172 });
}

Genome paramsGenome(vector<char*> args) {
    int c = stoi(args.front());
    args.erase(args.begin());
    vector<float> params;

    for (auto&& arg : args){
        params.push_back(stof(arg));
    }

    return Genome(c, params);
}

int main(int argc, char** argv) {
    vector<char*> args(argv + 1, argv + argc);
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        PlayerGenetic player(args.empty() ? senseiGenome() : paramsGenome(args));
        judge.play(player);
    }
}
