#include <random>
#include <array>
#include <iostream>

namespace SayiOyunu
{
    class Oynatici
    {
        public:
            explicit Oynatici();
            void printSayi(std::ostream &output) const;

        private:
            constexpr static const auto N = 4;
            
            void sayiTut();
            
            std::array<unsigned short, N> sayi;
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
    auto rand9 = std::uniform_int_distribution<unsigned short>{1, 9};
    auto rand10 = std::uniform_int_distribution<unsigned short>{0, 9};

    sayi[0] = rand9(generator);

    for (int i = 1; i < N; i++)
        sayi[i] = rand10(generator);
}