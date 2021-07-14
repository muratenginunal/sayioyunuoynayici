#include <random>
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

namespace SayiOyunu
{

    static constexpr const auto B = 10; //!< Tahminin temsil edildigi sayi tabani
    static constexpr const auto N = 4; //!< Tahminin basamak sayisi

    using TahminTipi = unsigned int;
    using BasamakTipi = unsigned short;
    using BasamaklarTasiyici = std::array<BasamakTipi, N>;
    using SonucSayisiTipi = unsigned short;

    class SayiTutucu
    {
        public:
            static auto tahminAl() -> TahminTipi;

        private:
            static auto tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici;

        public:
            explicit SayiTutucu();

            void sayiYazdir(std::ostream &output) const;
            bool birAdim(std::ostream &output) const;
        private:
            void sayiTut();
            auto tahminiDegerlendir(TahminTipi tahmin) const -> std::pair<SonucSayisiTipi, SonucSayisiTipi>;
            
            BasamaklarTasiyici basamaklar;
    };

    class Oyun
    {
        public:
            static auto getBUzeriNe1() -> TahminTipi;

        public:
            explicit Oyun();

            void oyna();

        private:
            SayiTutucu sayiTutucu;
    };
}

using namespace SayiOyunu;

int main()
{
    auto oyun = Oyun{};
    oyun.oyna();

    return 0;
}

using namespace std;

Oyun::Oyun()
{
    cout << "Merhaba! Sayi oyununa hosgeldiniz." << endl;
}

void Oyun::oyna()
{
    auto devam = true;
    while(devam)
    {
        devam = sayiTutucu.birAdim(cout);
    }
}

SayiTutucu::SayiTutucu()
{
    sayiTut();
}

bool SayiTutucu::birAdim(ostream &output) const
{
    auto tahmin = tahminAl();
    if (tahmin == 0)
        return false;

    output << char{8} << ' ' << endl;
    auto [arti, eksi] = tahminiDegerlendir(tahmin);
    output << "+" << arti << " -" << eksi << endl;
    if (arti == N)
    {
        cout << "TEBRİKLER, BİLGİSAYARIN TUTTUĞU SAYIYI BİLDİNİZ!";
        return false;
    }
    return true;
}

void SayiTutucu::sayiTut()
{
    auto tumOlasiliklar = vector<BasamakTipi>(B);
    for (int i=0; i < B; i++)
        tumOlasiliklar[i] = i;

    auto rd = random_device{};
    auto generator = mt19937{rd()};
    auto randBe1 = uniform_int_distribution<BasamakTipi>{1, B - 1};
    basamaklar[0] = randBe1(generator);
    tumOlasiliklar.erase(remove(tumOlasiliklar.begin(), tumOlasiliklar.end(), basamaklar[0]), tumOlasiliklar.end());
    for (int i = 1; i < N; i++)
    { 
        auto randRest = uniform_int_distribution<BasamakTipi>{0, static_cast<BasamakTipi>(B - 1 - i)};
        auto olasiSiraNo = randRest(generator);
        basamaklar[i] = tumOlasiliklar[olasiSiraNo];
        tumOlasiliklar.erase(remove(tumOlasiliklar.begin(), tumOlasiliklar.end(), basamaklar[i]), tumOlasiliklar.end());
    }
}

void SayiTutucu::sayiYazdir(ostream &output) const
{
    for(auto basamak: basamaklar)
        output << basamak;

    output << endl; 
}

auto SayiTutucu::tahminAl() -> TahminTipi
{
    auto tahmin = static_cast<TahminTipi>(-1);
    cin >> tahmin;    
    return tahmin;
}

auto SayiTutucu::tahminiDegerlendir(TahminTipi tahmin) const -> pair<SonucSayisiTipi, SonucSayisiTipi>
{
    auto arti = SonucSayisiTipi{0};
    auto eksi = SonucSayisiTipi{0};
    auto tahminBasamaklar = tahminiCoz(tahmin);
    for(int i=0; i < N; i++)
    {
        if(tahminBasamaklar[i] == basamaklar[i])
        {
            ++arti;
            continue;
        }

        if(find(basamaklar.cbegin(), basamaklar.cend(), tahminBasamaklar[i]) != basamaklar.cend())
            ++eksi;
    }
    assert(arti + eksi <= N);
    return {arti, eksi};
}

auto SayiTutucu::tahminiCoz(TahminTipi tahmin) -> BasamaklarTasiyici
{
    auto retVal = BasamaklarTasiyici{};
    auto bolen = Oyun::getBUzeriNe1();
    for(int i=0; i< N; i++)
    {
        retVal[i] = tahmin / bolen;
        tahmin -= retVal[i] * bolen;
        bolen /= B;
    }
    return retVal;
}

auto Oyun::getBUzeriNe1() -> TahminTipi
{
    auto retval = TahminTipi{1};
    for(int i = 0; i < N - 1; i++)
        retval *= B;

    return retval;
}
