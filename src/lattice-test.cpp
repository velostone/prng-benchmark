#include <boost/random/taus88.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "mcml_mcg59.h"
#include "next_floating_point.hpp"
#include "splitmix64.hpp"
#include "xorshift128plus.hpp"

#define PIC_SIZE 512
#define N_NUMBERS 1048576
#define SEED 20260224

#define TAUS88
#define MT11213B
#define MT19937
#define _XORSHIFT128PLUS
#define _MCG59

#define MCG59_C     302875106592253
#define MCG59_M     576460752303423488
#define MCG59_DEC_M 576460752303423487

double NextMCG59(MCG59* randomGenerator) {
	randomGenerator->value = (randomGenerator->value * randomGenerator->offset) & MCG59_DEC_M;
	return (double)(randomGenerator->value) / MCG59_M;
}

void write_in_file(int* picture, std::string filename) {
  std::ofstream ofile(filename);
  std::cout << "Start writing in " + filename + "\n";

  for (int i = 0; i < PIC_SIZE * PIC_SIZE; ++i) {
    ofile << picture[i] << " ";
  }
  ofile << "\n";
  ofile.close();

  std::cout << "Done!\n";
}

int main() {

  int* pic = new int[PIC_SIZE * PIC_SIZE];
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));

#ifdef TAUS88
	boost::random::taus88 taus88(SEED);
  boost::random::uniform_real_distribution<double> dis(0.0, 1.0);

  for (int i = 0; i < N_NUMBERS; ++i) {
		auto val = dis(taus88);
    int x = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    val = dis(taus88);
    int y = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    pic[y * PIC_SIZE + x] += 1;
  }

  write_in_file(pic, "..\\results\\lattice-test\\taus88_" + std::to_string(PIC_SIZE) + "_double.txt");
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));
#endif

#ifdef MT11213B
	boost::random::mt11213b mt11213b(SEED);
  boost::random::uniform_real_distribution<double> dist(0.0, 1.0);

  for (int i = 0; i < N_NUMBERS; ++i) {
		auto val = dist(mt11213b);
    int x = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    val = dist(mt11213b);
    int y = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    pic[y * PIC_SIZE + x] += 1;
  }

  write_in_file(pic, "..\\results\\lattice-test\\mt11213b_" + std::to_string(PIC_SIZE) + "_double.txt");
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));
#endif

#ifdef MT19937
	std::mt19937 mt19937(SEED);
  std::uniform_real_distribution<double> distr(0.0, 1.0);

  for (int i = 0; i < N_NUMBERS; ++i) {
		auto val = distr(mt19937);
    int x = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    val = distr(mt19937);
    int y = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    pic[y * PIC_SIZE + x] += 1;
  }

  write_in_file(pic, "..\\results\\lattice-test\\mt19937_" + std::to_string(PIC_SIZE) + "_double.txt");
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));
#endif

#ifdef _XORSHIFT128PLUS
	XorShift128Plus xorshift128plus(SEED);

  for (int i = 0; i < N_NUMBERS; ++i) {
		auto val = nextDouble<XorShift128Plus>(xorshift128plus);
    int x = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    val = nextDouble<XorShift128Plus>(xorshift128plus);
    int y = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    pic[y * PIC_SIZE + x] += 1;
  }

  write_in_file(pic, "..\\results\\lattice-test\\xorshift128plus_" + std::to_string(PIC_SIZE) + "_double.txt");
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));
#endif

#ifdef _MCG59
  MCG59 mcg59;
	InitMCG59(&mcg59, SEED, 0, 1);

  for (int i = 0; i < N_NUMBERS; ++i) {
		auto val = NextMCG59(&mcg59);
    int x = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    val = NextMCG59(&mcg59);
    int y = std::min(static_cast<int>(val * PIC_SIZE), PIC_SIZE - 1);

    pic[y * PIC_SIZE + x] += 1;
  }

  write_in_file(pic, "..\\results\\lattice-test\\mcg59_" + std::to_string(PIC_SIZE) + "_double.txt");
  memset(pic, 0, PIC_SIZE * PIC_SIZE * sizeof(int));
#endif
}