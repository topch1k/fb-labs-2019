#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/multiprecision/random.hpp>
#include <utility>      // std::pair, std::make_pair
#include <boost/integer/mod_inverse.hpp>
#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
//#include <boost/random/random_device.hpp>
using namespace boost::multiprecision;
using namespace boost::random;

bool MLprimTest(cpp_int NumberToCheck, int RoundsAmount); // Miller-Rabin primality test
cpp_int GetPrimeInRange(cpp_int MinBound, cpp_int MaxBound); // Get prime number in range [MinBound; MaxBound]
std::pair<cpp_int, cpp_int> GenerateSimpleNumberPair(cpp_int MinBound, cpp_int MaxBound); // Generate simple numbers pair;

// Generate private and public keys; PrivateKey (d, p, q); PublicKey (e, n);
void GenerateKeyPair(std::pair<cpp_int, std::pair<cpp_int, cpp_int>>& PrivateKey, 
						std::pair<cpp_int, cpp_int>& PublicKey, std::pair<cpp_int, cpp_int > SimpleNumbers);

cpp_int Encrypt(cpp_int Message, std::pair<cpp_int, cpp_int> PublicKey); // Encryption function
																												
cpp_int Decrypt(cpp_int EncMessage, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKey);  // Decryption function 

std::pair<cpp_int, cpp_int> Sign(cpp_int Message, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKey); //Create Digital sign
bool Verify(std::pair<cpp_int, cpp_int> Sign, std::pair<cpp_int, cpp_int> PublicKey); // Digital sign verification function

std::pair<cpp_int, cpp_int> SendKey(std::pair<cpp_int, cpp_int> PubKeyA, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrvtKeyA, 
											std::pair<cpp_int, cpp_int> PubKeyB, cpp_int SecValue); //SendKey function for
																								   //confidential key distribution protocol
bool ReceiveKey(std::pair<cpp_int, cpp_int> PubKeyB, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrvtKeyB,
	std::pair<cpp_int, cpp_int> PubKeyA, std::pair<cpp_int, cpp_int> Notification);