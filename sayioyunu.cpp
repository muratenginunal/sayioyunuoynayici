#include <random>
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

namespace SayiOyunu
{
    class Oynatici
    {
        public:
            explicit Oynatici();

            void printSayi(std::ostream &output) const;

        private:
            constexpr static const auto N = 4;
            using BasamakTipi = unsigned short;

            void sayiTut();
            
            std::array<BasamakTipi, N> sayi;
    }
}

using namespace SayiOyunu;

int main()
{
    auto oynatici = Oynatici{};
    oynatici.printSayi(std::cout);
    return 0;
}


Oynatici::Oynatici()
{
    sayiTut();
}

void Oynatici::sayiTut()
{
    auto generator = std::mt19937{0};
    auto rand9 = std::uniform_int_distribution<BasamakTipi>{1, 9};

    sayi[0] = rand9(generator);

    auto olasiBasamaklar = std::vector<BasamakTipi>{};

    for (int i = 1; i < N; i++)
    {        
        assert(std::is_sorted(olasiBasamaklar.cbegin(), olasiBasamaklar.cend()));

        auto randRest = std::uniform_int_distribution<BasamakTipi>{0, 10 - i};
        auto olasiSiraNo = randRest(generator);

        auto itOlasi = std::upper_bound(olasiBasamaklar.cbegin(), olasiBasamaklar.cend(), olasiSiraNo);
        assert(itOlasi >= olasiBasamaklar.cbegin());

        sayi[i] = olasiSiraNo + (itOlasi - olasiBasamaklar.cbegin());
        assert(sayi[i] < 10);
        assert(!std::binary_search(olasiBasamaklar.cbegin(), olasiBasamaklar.cend(), sayi[i]));

        auto itCikanlar = std::upper_bound(olasiBasamaklar.cbegin(), olasiBasamaklar.cend(), olasiSiraNo);
        olasiBasamaklar.insert(itCikanlar, sayi[i]);
    }
}