#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <algorithm>
#include <cassert>

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


struct Attributes
{
	int hp;
	int speed;
	int power;
};

class Weapon
{
public:
	Weapon(const std::string& name, int rank)
		:
		name(name),
		rank(rank)
	{}
	const std::string& GetName() const
	{
		return name;
	}
	int GetRank() const
	{
		return rank;
	}
	virtual int CalculateDamage(const Attributes& attr, Dice& d) const = 0;
private:
	std::string name;
	int rank;
};

class Fists : public Weapon
{
public:
	Fists()
		:
		Weapon("fists", 0)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{

		return attr.power + d.Roll(2);
	}

};

class Knife : public Weapon
{
public:
	Knife()
		:
		Weapon("knife", 2)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{

		return attr.speed * 3 + d.Roll(3);
	}

};

class Bat : public Weapon
{
public:
	Bat()
		:
		Weapon("bat", 1)
	{}
	int CalculateDamage(const Attributes& attr, Dice& d) const override
	{

		return attr.power * 2 + d.Roll(1);
	}

};

class MemeFighter
{

public:
	std::string GetName() const
	{
		return name;
	}
	int GetHp() const
	{
		return attr.hp;
	}
	int GetInitiative() const
	{
		return attr.speed + Roll(2);
	}
	bool IsAlive() const
	{
		return (attr.hp > 0);
	}
	void Attack(MemeFighter& other) const
	{
		if (other.IsAlive() && IsAlive()) {
			//assert(pWeapon != nullptr);

			ApplyDamageTo(other, pWeapon->CalculateDamage(attr, d));
			std::cout << name << " attacks " << other.GetName() << " with his " << pWeapon->GetName() << " for " << ".It has now " << other.GetHp() << " hp.\n";
		}
		else {
			std::cout << other.GetName() << " is already DEAD!\n";
		}
	}
	const Attributes& GetAttributes() const
	{
		return attr;
	}
	virtual void Tick()
	{
		if (IsAlive()) {
			int amt = Roll(1);
			attr.hp += amt;
			std::cout << name << " restored " << amt << "!!! Has now " << attr.hp << "\n";
		}
	}
	virtual void SpecialMove(MemeFighter&) = 0 {}
	virtual ~MemeFighter() 
	{
		delete pWeapon;
	}
	void GiveWeapon(Weapon* pNewWeapon)
	{
		delete pWeapon;
		pWeapon = pNewWeapon;
	}
	Weapon* PilferWeapon()
	{
		auto pWep = pWeapon;
		pWeapon = nullptr;
		return pWep;
	}

protected:
	MemeFighter(const std::string& name, int hp, int speed, int power, Weapon* pWeapon = nullptr)
		:
		name(name),
		attr({ hp,speed, power })
	{
		std::cout << name << " enters the ring!\n";
	}
	int Roll(int nDice = 1) const
	{
		return d.Roll(nDice);
	}
	void ApplyDamageTo(MemeFighter& target, int dmg) const
	{
		target.attr.hp -= dmg;

		std::cout << target.name << " takes " << dmg << " damage.\n";
		if (!target.IsAlive())
		{
			std::cout << target.name << " is dead!\n";
		}
	}

protected:
	std::string name;
	Attributes attr;

private:
	mutable Dice d;
	Weapon* pWeapon = nullptr;

};


class MemeFrog : public MemeFighter
{
public:
	MemeFrog(const std::string& name, Weapon* pWeapon = nullptr)
		:
		MemeFighter(name, 69, 7, 14, pWeapon)
	{}
	~MemeFrog(){}
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
	void Tick() override 
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
	MemeStoner(const std::string& name, Weapon* pWeapon = nullptr)
		:
		MemeFighter(name, 80, 4, 10, pWeapon)
	{}
	~MemeStoner(){}
	void SpecialMove(MemeFighter& target) override 
	{
		if (IsAlive()) 
		{
			if (Roll() % 2 == 0) 
			{
				name += "Super";
				attr.hp += 10;
				attr.speed += 3;
				attr.power = attr.power * 69 / 42;
				std::cout << name << " special moved!!!! It has now " << attr.hp << " hp " << " power " << attr.power << " speed " << attr.speed << "!!!!\n";
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
	p1->Attack( *p2 );
	p2->Attack( *p1 );
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
		new MemeFrog("Dat Boi", new Fists),
		new MemeStoner("Good Guy Greg", new Bat),
		new MemeFrog("the WB Frog", new Knife)
	};
	
	std::vector<MemeFighter*> t2 = {
		new MemeStoner("Chong", new Fists),
		new MemeStoner("Scumbag Steve", new Bat),
		new MemeFrog("Pepe", new Knife)
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