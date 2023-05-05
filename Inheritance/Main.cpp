#include <iostream>
#include <string>
#include <conio.h>
#include <random>


class Dice
{
public:
	int Roll(int nDice)
	{
		int total = 0;
		for (int n = 0; n < nDice; n++)
		{
			total += d6(rng);
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>(1, 6);
	std::mt19937 rng = std::mt19937(std::random_device{}());
};

class MemeFighter
{
protected:
	std::string name;
	int hp;
	int speed;
	int power;
	
private:
	mutable Dice d;
public:
	std::string GetName() const
	{
		return name;
	}
	int GetHp() const
	{
		return hp;
	}
	int GetInitiative() const
	{
		return speed + Roll(2);
	}
	bool IsAlive() const
	{
		return (hp > 0);
	}
	void Punch(MemeFighter& other) const
	{
		if (other.IsAlive() && IsAlive()) {
			int p = power + Roll(2);
			other.DeleteHp(p);
			std::cout << name << " punched " << other.GetName() << " for " << p << ". It has now " << other.GetHp() << " hp.\n";
		}
		else {
			std::cout << other.GetName() << " is already DEAD!\n";
		}
	}
	void Tick()
	{
		if (IsAlive()) {
			int amt = Roll(1);
			hp += amt;
			std::cout << name << " restored " << amt << "!!! Has now " << hp << "\n";
		}
	}
	virtual void SpecialMove(MemeFighter&) = 0 {}

protected:
	MemeFighter(const std::string& name, int hp, int speed, int power)
		:
		name(name),
		hp(hp),
		speed(speed),
		power(power)
	{
		std::cout << name << " enters the ring!\n";
	}
	int Roll(int nDice = 1) const
	{
		return d.Roll(nDice);
	}
	void DeleteHp(int amt)
	{
		hp -= amt;
	}
	void ApplyDamageTo(MemeFighter& target, int dmg) const
	{
		target.hp -= dmg;
		std::cout << target.name << " takes " << dmg << " damage.\n";
		if (!target.IsAlive())
		{
			std::cout << target.name << " is dead!\n";
		}
	}


};


class MemeFrog : public MemeFighter
{
public:
	MemeFrog(const std::string& name)
		:
		MemeFighter(name, 69, 7, 14)
	{}

	void SpecialMove(MemeFighter& target) override  
	{
		if (IsAlive() && target.IsAlive()) {

			if (Roll() % 3 == 0) {
				ApplyDamageTo(target, 20 + Roll(3));
				std::cout << name << " extra attacked " << target.GetName() << "!!! It has now " << target.GetHp() << " hp!!!!\n";
			}
			else {
				std::cout << "Special Move didn't work for " << name << "!!\n";
			}
		}
	}
	void Tick()
	{
		if (IsAlive())
		{
			std::cout << GetName() << " is hurt by the bad AIDS!\n";
			ApplyDamageTo(*this, Roll());
			MemeFighter::Tick();
		}
	}
};
 
class MemeStoner : public MemeFighter
{
public:
	MemeStoner(const std::string& name)
		:
		MemeFighter(name, 80, 4, 10)
	{}
	void SpecialMove(MemeFighter& target) override 
	{
		if (IsAlive()) 
		{
			if (Roll() % 2 == 0) 
			{
				name += "Super";
				hp += 10;
				speed += 3;
				power = power * 69 / 42;
				std::cout << name << " special moved!!!! It has now " << hp << " hp " << " power " << power << " speed " << speed << "!!!!\n";
			}
			else {
				std::cout << "Special Move didn't work for " << name << "!!\n";
			}
		}
	}
};



void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Punch( *p2 );
	p2->Punch( *p1 );
}

void DoSpecials(MemeFighter& f1, MemeFighter& f2)
{
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if (p1->GetInitiative() < p2->GetInitiative())
	{
		std::swap(p1, p2);
	}
	// execute attacks
	p1->SpecialMove(*p2);
	p2->SpecialMove(*p1);
}

int main()
{
	MemeFrog f1( "Dat Boi" );
	MemeStoner f2( "Good Guy Greg" );

	while( f1.IsAlive() && f2.IsAlive() )
	{
		// trade blows
		Engage( f1,f2 );
		// special moves
		DoSpecials(f1, f2);
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		std::cout << "Press any key to continue...";
		while( !_kbhit() );
		_getch();
		std::cout << std::endl << std::endl;
	}

	if( f1.IsAlive() )
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else
	{
		std::cout << f2.GetName() << " is victorious!";
	}
	while( !_kbhit() );
	return 0;
}