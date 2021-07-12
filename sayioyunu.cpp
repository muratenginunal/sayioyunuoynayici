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

            static constexpr const auto B = 10; //!< Tahminin temsil edildigi sayi tabani
            static constexpr const auto N = 4;  //!< Tahminin basamak sayisi

        private:
            static auto getBUzeriNe1() -> TahminTipi;

        public:
            using SonucSayisiTipi = unsigned short;

            explicit Oynatici();
            
            void sayiYazdir(std::ostream &output) const;
            auto tahminiDegerlendir(TahminTipi tahmin) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>;

        private:
            using BasamakTipi = unsigned short;
            using BasamaklarTasiyici = std::array<BasamakTipi, N>;

            void sayiTut();
            auto tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici;
            
            BasamaklarTasiyici basamaklar;
    }
}

using namespace SayiOyunu;

int main()
{
    auto oynatici = Oynatici{};
    for(Oynatici::TahminTipi tahmin = Oynatici::tahminAl(); tahmin != 0; tahmin = Oynatici::tahminAl())
    {
        auto [arti, eksi] = oynatici.tahminiDegerlendir(tahmin);
        if (arti == Oynatici::N)
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
    auto randBe1 = std::uniform_int_distribution<BasamakTipi>{1, B - 1};

    basamaklar[0] = randBe1(generator);

    auto simdiyeDekCikanlar = std::vector<BasamakTipi>{};
    simdiyeDekCikanlar.reserve(N);
    simdiyeDekCikanlar.push_back(basamaklar[0]);

    for (int i = 1; i < N; i++)
    {        
        auto randRest = std::uniform_int_distribution<BasamakTipi>{0, B - 1 - i};
        auto olasiSiraNo = randRest(generator);

        auto itOlasi = std::upper_bound(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), olasiSiraNo);
        assert(itOlasi >= simdiyeDekCikanlar.cbegin());

        basamaklar[i] = olasiSiraNo + (itOlasi - simdiyeDekCikanlar.cbegin());
        assert(basamaklar[i] < B);
        assert(!std::binary_search(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), basamaklar[i]));

        auto itCikanlar = std::upper_bound(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend(), basamaklar[i]);
        simdiyeDekCikanlar.insert(itCikanlar, basamaklar[i]);

        assert(std::is_sorted(simdiyeDekCikanlar.cbegin(), simdiyeDekCikanlar.cend()));
    }
}

void Oynatici::printSayi(std::ostream &output) const
{
    for(auto basamak: basamaklar)
        output << basamak;

    output << std::endl; 
}

auto Oynatici::tahminAl() -> TahminTipi
{
    auto tahmin = TahminTipi{-1};
    do
    {
        std::cout << "Programdan cikmak icin 0, rakamlari birbirinden farkli " << N << " basamakli, " << B << " tabaninda yazilmis bir sayi giriniz:" << std::endl;
        std::cin >> tahmin;
    }while(!tahmin || (tahminMin() < tahmin && tahmin < tahminMax());
    return tahmin;
}

auto Oynatici::tahminiDegerlendir(TahminTipi tahmin) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>
{
    auto arti = SonucSayisiTipi{0};
    auto eksi = SonucSayisiTipi{0};
    auto tahminBasamaklar = tahminiCoz(tahmin);
    for(i=0; i<N; i++)
    {
        if(tahminBasamaklari[i] == basamaklar[i])
        {
            ++arti;
            continue;
        }

        if(std::find(basamaklar.cbegin(), basamaklar.cend(), tahminBasamaklar[i]) != basamaklar.cend())
            ++eksi;
    }
    assert(arti + eksi <= N)
    return {arti, eksi};
}

auto Oynatici::tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici
{
    auto retVal = BasamaklarTasiyici{};
    auto bolen = Oynatici::getTabanUzeriNe1();
    for(int i=0; i<N; i++)
    {
        retVal[i] = tahmin / bolen;
        tahmin -= retVal[i] * bolen;
        bolen /= B;
    }
}

auto Oynatici::getBUzeriNe1() -> TahminTipi
{
    auto retval = TahminTipi{1};
    for(int i = 0; i < N-1; i++)
        retval *= B;

    return retval;
}