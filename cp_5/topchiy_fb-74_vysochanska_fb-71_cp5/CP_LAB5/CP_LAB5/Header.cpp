#include "Header.h"

bool MLprimTest(cpp_int NumberToCheck, int RoundsAmount)
{	
	 // p - 1 = d * 2^s; 
	// NumberToCheck - 1 = d * 2^s; d is odd
	cpp_int NumberToCheckCopy = NumberToCheck - 1;
	int s = 0;
	while (NumberToCheckCopy % 2 == 0) {
		s++;
		NumberToCheckCopy /= 2;
	}
	cpp_int d = NumberToCheckCopy; // d is odd
	//std::time_t now = std::time(0);
	//boost::random::mt19937 gen{ static_cast<std::uint32_t>(now) }; //Pseudo-random number generator 
	std::time_t now = std::time(0);
	typedef independent_bits_engine<mt19937, 256, cpp_int> generator_type;
	generator_type gen{ static_cast<std::uint32_t>(now) };

	cpp_int TempRandomNumber;
	cpp_int X;
	for (int i = 0; i < RoundsAmount; i++) {
		TempRandomNumber = (1 + gen()) % NumberToCheck - 2;
		//std::cout << TempRandomNumber << std::endl;
		X = powm(TempRandomNumber, d, NumberToCheck);
		if (X == 1 || X == NumberToCheck - 1) continue;
		for (int j = 0; j < s - 1; j++) {
			X = powm(X, 2, NumberToCheck);
			if (X == 1) return 0;
			else if (X == NumberToCheck - 1) break;
		}
		return 0;
	}
	return 1;
}

cpp_int GetPrimeInRange(cpp_int MinBound, cpp_int MaxBound)
{
	cpp_int Number;
	std::time_t now = std::time(0);
	typedef independent_bits_engine<mt19937, 256, cpp_int> generator_type;
	generator_type gen{ static_cast<std::uint32_t>(now) };
	do {
		Number = (gen() % MaxBound + MinBound) % MaxBound;
		//std::cout << Number << std::endl;
	} while (!MLprimTest(Number, 15));
	return Number;
}

std::pair<cpp_int, cpp_int> GenerateSimpleNumberPair(cpp_int MinBound, cpp_int MaxBound)
{	
	cpp_int CoreP, CoreQ, p, q; // p = 2*i*CoreP + 1, q = 2*i*CoreQ + 1; i = 1, 2...
	CoreP = GetPrimeInRange(MinBound, MaxBound);
	//std::cout << "CoreP " << std::endl;
	CoreQ = GetPrimeInRange(MinBound, MaxBound);
	//std::cout << "CoreQ " << std::endl;
	while (CoreP == CoreQ) {
		CoreQ = GetPrimeInRange(MinBound, MaxBound);
	}
	int i = 1;
	do {
		p = 2 * i * CoreP + 1;
		i++;
		std::cout << " Pi = " << p << std::endl;
		std::cout << i << "\t";
	} while (!MLprimTest(p, 10));
	std::cout << std::endl;
	i = 1; // Zeroing counter
	do {
		q = 2 * i * CoreQ + 1;
		i++;
		std::cout << " Qi = " << q << std::endl;
		std::cout << i << "\t";
	} while (!MLprimTest(q, 10));
	std::cout << std::endl;
	return std::pair<cpp_int, cpp_int>(p, q);
}

void GenerateKeyPair(std::pair<cpp_int, std::pair<cpp_int, cpp_int>>& PrivateKey, 
						std::pair<cpp_int, cpp_int>& PublicKey, std::pair<cpp_int, cpp_int > SimpleNumbers) 
{
	cpp_int n; // n = p * q; 
	n = SimpleNumbers.first * SimpleNumbers.second;
	cpp_int fi; // Euler function; fi(n) = (p-1)*(q-1)
	fi = (SimpleNumbers.first - 1) * (SimpleNumbers.second - 1);
	cpp_int e = pow((cpp_int)2, 16) + 1; // e = 2^16 + 1
	std::cout  << "gcd(e, fi) = " << gcd(e, fi) << std::endl; // For test
	cpp_int d = boost::integer::mod_inverse(e, fi);
	std::cout << "e*d mod fi = " << (e * d) % fi << std::endl; //For test
	//PrivateKey: <d, <p, q>>; SimpleNumbers : <p, q>;
	PrivateKey.first = d;
	PrivateKey.second.first = SimpleNumbers.first;
	PrivateKey.second.second = SimpleNumbers.second;
	// PublicKey : <n, e>;
	PublicKey.first = n;
	PublicKey.second = e;
}

cpp_int Encrypt(cpp_int Message, std::pair<cpp_int, cpp_int> PublicKey)
{
	//PublicKey: (n, e);
	cpp_int EncMessage; //Encrypted message 
	EncMessage = powm(Message, PublicKey.second, PublicKey.first); // C = M^e mod n 
	return EncMessage;
}

cpp_int Decrypt(cpp_int EncMessage, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKey)
{
	cpp_int DecMessage; //Decrypted message
	cpp_int n = PrivateKey.second.first * PrivateKey.second.second; // n = p * q;
	DecMessage = powm(EncMessage, PrivateKey.first, n);
	return DecMessage;
}

std::pair<cpp_int, cpp_int> Sign(cpp_int Message, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKey)
{	
	cpp_int Sign; // S = M^d mod n;
	//PrivateKey: <d, <p, q>>
	cpp_int n = PrivateKey.second.first * PrivateKey.second.second;
	Sign = powm(Message, PrivateKey.first, n);
	return std::pair<cpp_int, cpp_int>(Message, Sign);
}

bool Verify(std::pair<cpp_int, cpp_int> Sign, std::pair<cpp_int, cpp_int> PublicKey)
{
	cpp_int DecSign;
	// PublicKey: <n, e>;
	DecSign = powm(Sign.second, PublicKey.second, PublicKey.first);
	if (Sign.first = DecSign) return 1;
	else return 0;
}

std::pair<cpp_int, cpp_int> SendKey(std::pair<cpp_int, cpp_int> PubKeyA, 
	std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrvtKeyA, std::pair<cpp_int, cpp_int> PubKeyB, cpp_int SecValue)
{
	//S = k(SecValue)^d mod n;
	cpp_int d = PrvtKeyA.first;
	cpp_int n = PubKeyA.first;
	cpp_int S = powm(SecValue, d, n);
	//PublicKey: <n, e>
	//PrivateKey: <d, <p, q>>
	cpp_int e1 = PubKeyB.second;
	cpp_int n1 = PubKeyB.first;
	// k1 = k(SecValue)^e1 mod n1;
	cpp_int k1 = powm(SecValue, e1, n1);
	// S1 = S^e1 mod n1;
	cpp_int S1 = powm(S, e1, n1);
	return std::pair<cpp_int, cpp_int>(k1, S1);
}

bool ReceiveKey(std::pair<cpp_int, cpp_int> PubKeyB, std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrvtKeyB,
	std::pair<cpp_int, cpp_int> PubKeyA, std::pair<cpp_int, cpp_int> Notification)
{
	//PrvtKey: <d, <p, q>>
	cpp_int d1 = PrvtKeyB.first;
	//Notification: <k1, S1>
	cpp_int k1 = Notification.first;
	cpp_int S1 = Notification.second;
	cpp_int n1 = PubKeyB.first;
	cpp_int SecValue = powm(k1, d1, n1);
	cpp_int S = powm(S1, d1, n1);
	cpp_int e = PubKeyA.second;
	cpp_int n = PubKeyA.first;
	if (SecValue == powm(S, e, n)) return 1;
	else return 0;
}

