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

using namespace std;

Oynatici::Oynatici()
{
    cout << "Merhaba! Sayi oyununa hosgeldiniz." << endl;
    cout << "Oynatici::N : " << N << endl;
    cout << "Oynatici::B : " << B << endl;
    sayiTut();
    sayiYazdir(cout);
}

void Oynatici::oyna()
{
    for(TahminTipi tahmin = tahminAl(); tahmin != 0; tahmin = tahminAl())
    {
        auto [arti, eksi] = tahminiDegerlendir(tahmin);
        cout << "+" << arti << " -" << eksi << endl;
        if (arti == N)
            break;
    }
    sayiYazdir(cout);
}

void Oynatici::sayiTut()
{
    cout << "sayiTut basladi" << endl;

    auto tumOlasiliklar = vector<BasamakTipi>{};
    tumOlasiliklar.reserve(B);
    for (int i=0; i < B; i++)
        tumOlasiliklar[i] = i;

    auto generator = mt19937{0};
    auto randBe1 = uniform_int_distribution<BasamakTipi>{1, B - 1};
    cout << "raslantisal sayi ureticileri olusturuldu" << endl;
    basamaklar[0] = randBe1(generator);
    cout << "raslantisal sayi uretildi: " << basamaklar[0] << endl;
    tumOlasiliklar.erase(remove(tumOlasiliklar.begin(), tumOlasiliklar.end(), basamaklar[0]), tumOlasiliklar.end());
    for (int i = 1; i < N; i++)
    { 
        cout << "i: " << i << " , tum olasiliklar: ";
        for (auto a : tumOlasiliklar)
            cout << a << " ";
        cout << endl;
        
        auto randRest = uniform_int_distribution<BasamakTipi>{0, static_cast<BasamakTipi>(B - 1 - i)};
        auto olasiSiraNo = randRest(generator);
        basamaklar[i] = tumOlasiliklar[olasiSiraNo];
        tumOlasiliklar.erase(remove(tumOlasiliklar.begin(), tumOlasiliklar.end(), basamaklar[i]), tumOlasiliklar.end());
    }
}

void Oynatici::sayiYazdir(ostream &output) const
{
    for(auto basamak: basamaklar)
        output << basamak;

    output << endl; 
}

auto Oynatici::tahminAl() -> TahminTipi
{
    auto tahmin = static_cast<TahminTipi>(-1);
    
    do
    {
        cout << "Programdan cikmak icin 0, rakamlari birbirinden farkli " << N << " basamakli, " << B << " tabaninda yazilmis bir sayi giriniz:" << endl;
        cin >> tahmin;
        cout << "Girilen tahmin: " << tahmin << endl;
    }while(!tahmin /*|| (tahminMin() < tahmin && tahmin < tahminMax()*/);
    
    return tahmin;
}

auto Oynatici::tahminiDegerlendir(TahminTipi tahmin) const -> pair<SonucSayisiTipi, SonucSayisiTipi>
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

        if(find(basamaklar.cbegin(), basamaklar.cend(), tahminBasamaklar[i]) != basamaklar.cend())
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
