#include <random>
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utilities>

namespace SayiOyunu
{
    class Oynatici
    {
        public:
            using TahminTipi = unsigned int;

            static auto tahminAl() -> TahminTipi;
            static constexpr auto tahminMax() -> TahminTipi;
            static constexpr auto tahminMin() -> TahminTipi;
        private:
            static constexpr const auto N = 4;

        public:
            using SonucSayisiTipi = unsigned short;

            explicit Oynatici();
            
            void sayiYazdir(std::ostream &output) const;
            auto tahminiDegerlendir(TahminTipi) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>;

        private:
            using BasamakTipi = unsigned short;

            void sayiTut();
            
            std::array<BasamakTipi, N> sayi;
    }
}

using namespace SayiOyunu;

int main()
{
    auto oynatici = Oynatici{};
    for(Oynatici::TahminTipi tahmin = Oynatici::tahminAl(); tahmin != 0; tahmin = Oynatici::tahminAl())
    {
        auto [arti, eksi] = oynatici.tahminiDegerlendir(tahmin);
        if (arti == 4)
            break;
    }
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

    auto simdiyeDekCikanlar = std::vector<BasamakTipi>{};
    simdiyeDekCikanlar.reserve(N);
    simdiyeDekCikanlar.push_back(sayi[0]);

    for (int i = 1; i < N; i++)
    {        
        auto randRest = std::uniform_int_distribution<BasamakTipi>{0, 9 - i};
        auto olasiSiraNo = randRest(generator);

        auto itOlasi = std::upper_bound(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), olasiSiraNo);
        assert(itOlasi >= simdiyeDekCikanlar.cbegin());

        sayi[i] = olasiSiraNo + (itOlasi - simdiyeDekCikanlar.cbegin());
        assert(sayi[i] < 10);
        assert(!std::binary_search(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), sayi[i]));

        auto itCikanlar = std::upper_bound(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), sayi[i]);
        simdiyeDekCikanlar.insert(itCikanlar, sayi[i]);

        assert(std::is_sorted(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend()));
    }
}

void Oynatici::printSayi(std::ostream &output) const
{
    for(auto d: sayi)
        output << d;

    output << std::endl; 
}

auto Oynatici::tahminAl() -> TahminTipi
{
    auto tahmin = TahminTipi{-1};
    do
    {
        std::cout << "Programdan cikmak icin 0, rakamlari birbirinden farkli " << N << " basamakli bir sayi giriniz:" << std::endl;
        std::cin >> tahmin;
    }while(!tahmin || (tahminMin() < tahmin && tahmin < tahminMax());
    return tahmin;
}
