#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <algorithm>

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
	std::vector<MemeFighter*> t1 = {
		new MemeFrog("Dat Boi"),
		new MemeStoner("Good Guy Greg"),
		new MemeFrog("the WB Frog")
	};
	
	std::vector<MemeFighter*> t2 = {
		new MemeStoner("Chong"),
		new MemeStoner("Scumbag Steve"),
		new MemeFrog("Pepe")
	};

	const auto alive_pred = [](MemeFighter* pf) { return pf->IsAlive(); };
	while (
		std::any_of(t1.begin(), t1.end(), alive_pred) &&
		std::any_of(t2.begin(), t2.end(), alive_pred))
	{
		std::random_shuffle(t1.begin(), t1.end());
		std::partition(t1.begin(), t1.end(), alive_pred);
		std::random_shuffle(t2.begin(), t2.end());
		std::partition(t2.begin(), t2.end(), alive_pred);

		for (size_t i = 0; i < t1.size(); i++)
		{
			Engage(*t1[i], *t2[i]);
			DoSpecials(*t1[i], *t2[i]);
			std::cout << "------------------------------------" << std::endl;
		}
		std::cout << "=====================================" << std::endl;

		for (size_t i = 0; i < t1.size(); i++)
		{
			t1[i]->Tick();
			t2[i]->Tick();
		}
		std::cout << "=====================================" << std::endl;

		std::cout << "Press any key to continue...";
		while (!_kbhit());
		_getch();
		std::cout << std::endl << std::endl;
	}

	if (std::any_of(t1.begin(), t1.end(), alive_pred))
	{
		std::cout << "Team ONE is victorious!" << std::endl;
	}
	else
	{
		std::cout << "Team TWO is victorious!" << std::endl;
	}
	while (!_kbhit());

	for (size_t i = 0; i < t1.size(); i++)
	{
		delete t1[i];
		delete t2[i];
	}

	return 0;
}