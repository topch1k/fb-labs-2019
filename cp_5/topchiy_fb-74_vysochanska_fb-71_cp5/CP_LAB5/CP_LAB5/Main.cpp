#include "Header.h"
int main()
{
	srand(time(0));
	cpp_int a("35742549198872617291353508656626642567");
	cpp_int b("3574254919887261729135350865662664256778680092345");
	
	std::pair <cpp_int, cpp_int> SimpleNumbersPairA;
	std::pair <cpp_int, cpp_int> SimpleNumbersPairB;
	SimpleNumbersPairA = GenerateSimpleNumberPair(a, b);
	SimpleNumbersPairB = GenerateSimpleNumberPair(a, b);
	cpp_int FirstComposition = SimpleNumbersPairA.first * SimpleNumbersPairA.second;
	cpp_int SecondComposition = SimpleNumbersPairB.first * SimpleNumbersPairB.second;
	std::cout << SimpleNumbersPairA.first << "  " << SimpleNumbersPairA.second << std::endl;
	while (FirstComposition > SecondComposition) {
		SimpleNumbersPairB = GenerateSimpleNumberPair(a, b);
		SecondComposition = SimpleNumbersPairB.first * SimpleNumbersPairB.second;
	}
	std::cout << SimpleNumbersPairB.first << "  " << SimpleNumbersPairB.second << std::endl;
	// Values in report:
	/*cpp_int P("228478183506015368011317436896217482289154169757319");
	cpp_int Q("197459675612196052595333838411155196226754421040283");

	cpp_int P1("3516023237363950628763737068817847502593314979128479");
	cpp_int Q1("78340591635928942878772424149794241730218403991091");
	std::pair <cpp_int, cpp_int> SimpleNumbersPairA(P, Q);
	std::pair <cpp_int, cpp_int> SimpleNumbersPairB(P1, Q1);*/
	std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKeyA;
	std::pair<cpp_int, std::pair<cpp_int, cpp_int>> PrivateKeyB;
	std::pair<cpp_int, cpp_int> PublicKeyA;
	std::pair<cpp_int, cpp_int> PublicKeyB;
	GenerateKeyPair(PrivateKeyA, PublicKeyA, SimpleNumbersPairA);
	GenerateKeyPair(PrivateKeyB, PublicKeyB, SimpleNumbersPairB);
	std::cout << "A: d = " << PrivateKeyA.first << "\n" << " n = " << PublicKeyA.first << "\n" << " e = " << PublicKeyA.second << std::endl;
	std::cout << "B: d = " << PrivateKeyB.first << "\n" << " n = " << PublicKeyB.first << "\n" << " e = " << PublicKeyB.second << std::endl;
	//-------------------------------------------------------------------------------------
	cpp_int Message("0x8A323E34209BA7068FD7C190C132122F859ECA9F9C6057AADBCE86D58C2395CD");
	cpp_int EncMessage = Encrypt(Message, PublicKeyA);
	cpp_int DecMessage = Decrypt(EncMessage, PrivateKeyA);
	std::cout << "Message: " << Message<< std::endl; 
	std::cout << "EncMessage: " << EncMessage<< std::endl; 
	std::cout << "DecMessage: " << DecMessage<< std::endl; 
	//-------------------------------------------------------------
	std::pair<cpp_int, cpp_int> Signature;
	Signature = Sign(Message, PrivateKeyA);
	std::cout << "Signature: " << Signature.second << std::endl;
	std::cout << Verify(Signature, PublicKeyA) << std::endl;
	//-------------------------------------------------------------
	std::pair<cpp_int, cpp_int> Notif;
	Notif = SendKey(PublicKeyA, PrivateKeyA, PublicKeyB, Message);
	std::cout << "k1: " << Notif.first << "\nS1: " << Notif.second << std::endl;
	std::cout << ReceiveKey(PublicKeyB, PrivateKeyB, PublicKeyA, Notif) << std::endl;
	
	system("pause");
	return 0;
}