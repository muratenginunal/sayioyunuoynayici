#include <random>
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

namespace SayiOyunu
{
    class Oynatici
    {
        private:
            using TahminTipi = unsigned int;

            static auto tahminAl() -> TahminTipi;
//            static constexpr auto tahminMax() -> TahminTipi;
//            static constexpr auto tahminMin() -> TahminTipi;

            static constexpr const auto B = 10; //!< Tahminin temsil edildigi sayi tabani
            static constexpr const auto N = 4;  //!< Tahminin basamak sayisi

            using BasamakTipi = unsigned short;
            using BasamaklarTasiyici = std::array<BasamakTipi, N>;
            using SonucSayisiTipi = unsigned short;

            static auto getBUzeriNe1() -> TahminTipi;
            static auto tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici;

        public:
            explicit Oynatici();

            void oyna();            

        private:
            void sayiYazdir(std::ostream &output) const;
            auto tahminiDegerlendir(TahminTipi tahmin) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>;
            void sayiTut();
            
            BasamaklarTasiyici basamaklar;
    };
}

using namespace SayiOyunu;

int main()
{
    auto oynatici = Oynatici{};
    oynatici.oyna();
    return 0;
}

Oynatici::Oynatici()
{
    std::cout << "Merhaba! Sayi oyununa hosgeldiniz." << std::endl;
    std::cout << "Oynatici::N : " << N << std::endl;
    std::cout << "Oynatici::B : " << B << std::endl;
    sayiTut();
    sayiYazdir(std::cout);
}

void Oynatici::oyna()
{
    for(Oynatici::TahminTipi tahmin = Oynatici::tahminAl(); tahmin != 0; tahmin = Oynatici::tahminAl())
    {
        auto [arti, eksi] = oynatici.tahminiDegerlendir(tahmin);
        std::cout << "+" << arti << " -" << eksi << std::endl;
        if (arti == Oynatici::N)
            break;
    }
    oynatici.sayiYazdir(std::cout);
}

void Oynatici::sayiTut()
{
    std::cout << "sayiTut basladi" << std::endl;

    auto tumOlasiliklar = std::vector<BasamakTipi>{};
    tumOlasiliklar.reserve(B);
    for (int i=0; i < B; i++)
        tumOlasiliklar[i] = i;

    auto generator = std::mt19937{0};
    auto randBe1 = std::uniform_int_distribution<BasamakTipi>{1, B - 1};
    std::cout << "raslantisal sayi ureticileri olusturuldu" << std::endl;
    basamaklar[0] = randBe1(generator);
    std::cout << "raslantisal sayi uretildi: " << basamaklar[0] << std::endl;
    auto nRemoved = std::erase(tumOlasiliklar, basamaklar[0]);
    assert(nRemoved == 1);
    for (int i = 1; i < N; i++)
    {        
        std::cout << "i: " << i;
        auto randRest = std::uniform_int_distribution<BasamakTipi>{0, static_cast<BasamakTipi>(B - 1 - i)};
        auto olasiSiraNo = randRest(generator);
        basamaklar[i] = tumOlasiliklar[olasiSiraNo];
        auto nRemoved = std::erase(tumOlasiliklar, basamaklar[i]);
        assert(nRemoved == 1);
    }
}

void Oynatici::sayiYazdir(std::ostream &output) const
{
    for(auto basamak: basamaklar)
        output << basamak;

    output << std::endl; 
}

auto Oynatici::tahminAl() -> TahminTipi
{
    auto tahmin = static_cast<TahminTipi>(-1);
    
    do
    {
        std::cout << "Programdan cikmak icin 0, rakamlari birbirinden farkli " << N << " basamakli, " << B << " tabaninda yazilmis bir sayi giriniz:" << std::endl;
        std::cin >> tahmin;
        std::cout << "Girilen tahmin: " << tahmin << std::endl;
    }while(!tahmin /*|| (tahminMin() < tahmin && tahmin < tahminMax()*/);
    
    return tahmin;
}

auto Oynatici::tahminiDegerlendir(TahminTipi tahmin) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>
{
    auto arti = SonucSayisiTipi{0};
    auto eksi = SonucSayisiTipi{0};
    auto tahminBasamaklar = tahminiCoz(tahmin);
    for(int i=0; i<N; i++)
    {
        if(tahminBasamaklar[i] == basamaklar[i])
        {
            ++arti;
            continue;
        }

        if(std::find(basamaklar.cbegin(), basamaklar.cend(), tahminBasamaklar[i]) != basamaklar.cend())
            ++eksi;
    }
    assert(arti + eksi <= N);
    return {arti, eksi};
}

auto Oynatici::tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici
{
    auto retVal = BasamaklarTasiyici{};
    auto bolen = Oynatici::getBUzeriNe1();
    for(int i=0; i<N; i++)
    {
        retVal[i] = tahmin / bolen;
        tahmin -= retVal[i] * bolen;
        bolen /= B;
    }
    return retVal;
}

auto Oynatici::getBUzeriNe1() -> TahminTipi
{
    auto retval = TahminTipi{1};
    for(int i = 0; i < N-1; i++)
        retval *= B;

    return retval;
}
