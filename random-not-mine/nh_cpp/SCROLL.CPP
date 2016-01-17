#include "scroll.h"

scrollclass::scrollclass(){
	color=HI_PAPER; 
	otype=OTYPE_PAPER;
	merge=1;
}

scroll_of_enchant_armor::scroll_of_enchant_armor(){
	desc=sname[0];
	name="enchant armor";
	price=80;
}

void scroll_of_enchant_armor::read(){}

scroll_of_destroy_armor::scroll_of_destroy_armor(){
	desc=sname[1];
	name="destroy armor";
	price=100;
}

void scroll_of_destroy_armor::read(){}

scroll_of_confuse_monster::scroll_of_confuse_monster(){
	desc=sname[2];
	name="confuse monster";
	price=100;
}

void scroll_of_confuse_monster::read(){}

scroll_of_scare_monster::scroll_of_scare_monster(){
	desc=sname[3];
	name="scare monster";
	price=80;
}

void scroll_of_scare_monster::read(){}

scroll_of_remove_curse::scroll_of_remove_curse(){
	desc=sname[4];
	name="remove curse";
	price=80;
}

void scroll_of_remove_curse::read(){}

scroll_of_enchant_weapon::scroll_of_enchant_weapon(){
	desc=sname[5];
	name="enchant weapon";
	price=60;
}

void scroll_of_enchant_weapon::read(){}

scroll_of_create_monster::scroll_of_create_monster(){
	desc=sname[6];
	name="create monster";
	price=200;
}

void scroll_of_create_monster::read(){}

scroll_of_taming::scroll_of_taming(){
	desc=sname[7];
	name="taming";
	price=200;
}

void scroll_of_taming::read(){}

scroll_of_genocide::scroll_of_genocide(){
	desc=sname[8];
	name="genocide";
	price=300;
}

void scroll_of_genocide::read(){}

scroll_of_light::scroll_of_light(){
	desc=sname[9];
	name="light";
	price=50;
}

void scroll_of_light::read(){}

scroll_of_teleportation::scroll_of_teleportation(){
	desc=sname[10];
	name="teleportation";
	price=100;
}

void scroll_of_teleportation::read(){}

scroll_of_gold_detection::scroll_of_gold_detection(){
	desc=sname[11];
	name="gold detection";
	price=100;
}

void scroll_of_gold_detection::read(){}

scroll_of_food_detection::scroll_of_food_detection(){
	desc=sname[12];
	name="food detection";
	price=100;
}

void scroll_of_food_detection::read(){}

scroll_of_identify::scroll_of_identify(){
	desc=sname[13];
	name="identify";
	price=20;
}

void scroll_of_identify::read(){}

scroll_of_magic_mapping::scroll_of_magic_mapping(){
	desc=sname[14];
	name="magic mapping";
	price=100;
}

void scroll_of_magic_mapping::read(){}

scroll_of_amnesia::scroll_of_amnesia(){
	desc=sname[15];
	name="amnesia";
	price=200;
}

void scroll_of_amnesia::read(){}

scroll_of_fire::scroll_of_fire(){
	desc=sname[16];
	name="fire";
	price=100;
}

void scroll_of_fire::read(){}

scroll_of_punishment::scroll_of_punishment(){
	desc=sname[17];
	name="punishment";
	price=300;
}

void scroll_of_punishment::read(){}

scroll_of_charging::scroll_of_charging(){
	desc=sname[18];
	name="charging";
	price=300;
}

void scroll_of_charging::read(){}

scroll_of_blank_paper::scroll_of_blank_paper(){
	desc="unlabeled";
	name="blank paper";
	price=60;
}

void scroll_of_blank_paper::read(){}

scroll_of_mail::scroll_of_mail(){
	desc="stamped";
	name="mail";
	price=0;
}

void scroll_of_mail::read(){}


// This function generates a random scroll according to the probabilities
// using the same algorithm that is used in nethack 3.1.3

objectclass *randomscroll(){

	struct sinfo{
		objectclass *(*fp)();       // Function that creates the object
		uchar prob;                 // probability of object being created.
	};
	
	struct sinfo sinfo[]={
	// Put all the class create() function is here that are
	// possible candidates for this random object generator.
	// The sum of all the probabilities in this list must be 1000.
	// Think of it as XX.X% chance of being generated.
	{scroll_of_enchant_armor::create,63},
	{scroll_of_destroy_armor::create,45},
	{scroll_of_confuse_monster::create,53},
	{scroll_of_scare_monster::create,35},
	{scroll_of_remove_curse::create,65},
	{scroll_of_enchant_weapon::create,85},
	{scroll_of_create_monster::create,45},
	{scroll_of_taming::create,15},
	{scroll_of_genocide::create,15},
	{scroll_of_light::create,95},
	{scroll_of_teleportation::create,55},
	{scroll_of_gold_detection::create,33},
	{scroll_of_food_detection::create,25},
	{scroll_of_identify::create,185},
	{scroll_of_magic_mapping::create,45},
	{scroll_of_amnesia::create,35},
	{scroll_of_fire::create,48},
	{scroll_of_punishment::create,15},
	{scroll_of_charging::create,15},
	{scroll_of_blank_paper::create,28}
	// make sure the last one does not have a trailing comma
	};

	sshort prob;
	uchar count=0;

	prob=rnd0(1000);
	while((prob -= sinfo[count].prob) > 0) count++;
	return (sinfo[count].fp)();
}

// This function takes sname[] (declared in scroll.h) and moves the 
// structs around.  This algorithm may not be strictly accurate to 
// Nethack 3.1.3.  It should only be called once at the start of the game.

void randomizescrolls(){

	uchar numscrolls=sizeof(sname)/sizeof(sname[0]);
	char * tmp;
	uchar changeto=0;
	
	for(int i=0;i<numscrolls;i++){
		changeto=rnd0(numscrolls-1);  // because numscrolls is 1 based.
		// move destination to tmp
		tmp=sname[changeto];
		// move current index to destination
		sname[changeto]=sname[i];
		// move tmp to current index
		sname[i]=tmp;
	}
}

// Needs work to be more complete.

void scrollclass::printname(){

if (curseknown && cursed)
	cout << "cursed ";
else if(curseknown && blessed)
	cout << "blessed ";
else
	cout << "uncursed ";

if (nameknown)
	cout << "scroll of "<< name;
else
	cout << desc << " scroll";

//if (called)
//    cout << " called " << called;

cout << endl;

}


