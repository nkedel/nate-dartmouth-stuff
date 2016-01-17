#ifndef OBJECTS_H
#define OBJECTS_H

#include "global.h"

#define AMULET_CLASS    0
#define ARMOR_CLASS     1
#define BALL_CLASS      2
#define CHAIN_CLASS     3
#define FOOD_CLASS      4
#define GEM_CLASS       5
#define MONEY_CLASS     6
#define POTION_CLASS    7
#define RING_CLASS      8
#define ROCK_CLASS      9
#define SCROLL_CLASS    10
#define SPELLBOOK_CLASS 11
#define TOOL_CLASS      12
#define VENOM_CLASS     13
#define WAND_CLASS      14
#define WEAPON_CLASS    15
#define NUM_OCLASSES    15
#define RANDOM_CLASS    16
#define ILLEGAL_CLASS   17

#define OTYPE_LIQUID    1
#define OTYPE_WAX       2
#define OTYPE_VEGGY     3
#define OTYPE_FLESH     4
#define OTYPE_PAPER     5
#define OTYPE_CLOTH     6
#define OTYPE_LEATHER   7
#define OTYPE_WOOD      8
#define OTYPE_BONE      9
#define OTYPE_DRAGON    10
#define OTYPE_IRON      11      // includes steel
#define OTYPE_METAL     12
#define OTYPE_COPPER    13      // Includes brass
#define OTYPE_SILVER    14
#define OTYPE_GOLD      15
#define OTYPE_PLATINUM  16
#define OTYPE_MITHRIL   17
#define OTYPE_PLASTIC   18
#define OTYPE_GLASS     19
#define OTYPE_GEMSTONE  20
#define OTYPE_MINERAL   21
#define OTYPE_NOTHING   22      // initialization

#define ILLOBJ_SYM      ']'
#define WEAPON_SYM      ')'
#define ARMOR_SYM       '['
#define RING_SYM        '='
#define AMULET_SYM      '"'
#define TOOL_SYM        '('
#define FOOD_SYM        '%'
#define POTION_SYM      '!'
#define SCROLL_SYM      '?'
#define SPBOOK_SYM      '+'
#define WAND_SYM        '/'
#define GOLD_SYM        '$'
#define GEM_SYM         '*'
#define ROCK_SYM        '`'
#define BALL_SYM        '0'
#define CHAIN_SYM       '_'
#define VENOM_SYM       '.'

class monstclass;
class hero;

class objectclass {
protected:        
	Bitfields(otype,5);             // OTYPE_* type
	Bitfields(cursed,1);            // should both be 
	Bitfields(blessed,1);           // 0 for uncursed
	Bitfields(curseknown,1);        // Curse status known
	// One byte worth
	Bitfields(nameknown,1);         // name known
	Bitfields(wielded,1);           // Object wielded
	Bitfields(unique,1);            // should be only one?
	Bitfields(wishable,1);          // can wish for item?
	Bitfields(unpaid,1);            // need to paid for?
	Bitfields(oinvis,1);            // Object invisible.
	Bitfields(greased,1);           // Object greased.
	Bitfields(merge,1);             // should like objects be merged in pack?
	//Two bytes worth
	Bitfields(color,4);             // symbol color
	Bitfields(unused2,4);        
	// Three bytes worth

	char *name;                     // name
	char *desc;                     // description
	uchar packletter;               // current or last letter in inv.
									// this only works for an instantiation.
									// could be made static so it will work
									// for whole class, but monsters
									// picking up things will throw this off.
									// solution is to make monstpack that does
									// not take into account packletter, 
	ushort weight;
	ushort quantity;
	ushort price;                   // items price in shop (unmodified)
									// NOTE: this is not used to determine
									// if the object needs to be paid for,
									// only what the price would be if it
									// did.
public:
	objectclass();
	~objectclass(){}
	uchar gettype(){return (uchar)otype;}
	virtual uchar getclass(){return(ILLEGAL_CLASS);}
	void grease() {greased=1;}
	void ungrease() {greased=0;}
	void bless() {blessed=1;cursed=0;}
	void curse() {cursed=1;blessed=0;}
	void uncurs() {cursed=0;blessed=0;}
	uchar isgreased() {return (uchar)greased;}
	uchar iscursed() {return (uchar)cursed;}
	uchar isblessed() {return (uchar)blessed;}
	uchar isuncursed() {return (uchar) !(blessed||cursed);}
	void pay() {unpaid=0;}
	uchar isknown() {return (uchar)nameknown;}
	void makeknown() {nameknown=1;}
	void identify() {curseknown=1;nameknown=1;}
	// should be one for each class type
	virtual uchar isamulet() {return(FALSE);}
	virtual uchar isarmor()  {return(FALSE);}
	virtual uchar isball()  {return(FALSE);}
	virtual uchar ischain() {return(FALSE);}
	virtual uchar isfood() {return(FALSE);}
	virtual uchar isgem() {return(FALSE);}
	virtual uchar ismoney() {return(FALSE);}
	virtual uchar ispotion() {return(FALSE);}
	virtual uchar isring() {return(FALSE);}
	virtual uchar isrock() {return(FALSE);}
	virtual uchar isscroll() {return(FALSE);}
	virtual uchar isspellbook() {return(FALSE);}
	virtual uchar istool() {return(FALSE);}
	virtual uchar isvenom() {return(FALSE);}
	virtual uchar iswand() {return(FALSE);}
	virtual uchar isweapon() {return(FALSE);}
	virtual void printname(){}
	char *getname(){return(name);}
	char *getdesc(){return(desc);}
	char getletter(){return packletter;}
	void setletter(char let){packletter=let;}
	virtual void draw(){}
	objectclass *create(){return NULL;}
	virtual void weild(){}
	virtual void unwield(){}
	virtual void attack(){message("This doesn't work very well.");}
	virtual void invoke(){message("Nothing happens.");}
	virtual void loot(){message("Nothing to loot.");}
	virtual void apply(){message("This doesn't do anything useful.");}
	virtual void zap(){message("Nothing happens");}
	virtual void wear(){message("Nothing to wear");}
	virtual void puton(){message("Nothing to put on.");}
	virtual void offer(){message("Nothing to offer.");}
	virtual void kicked(){message("Ouch!");}
	virtual void dip(){message("It gets wet");}
	virtual void dipinto(objectclass *obj){message("You can't dip it in this!");}
	virtual void quaff(monstclass *mon){message("You can't drink this!");}
	virtual void breathe(monstclass *mon){}
	virtual void charge(){message("Charging this doesn't work");}
	virtual void read(){message("You can't read this!");}
	// should be called any time an object is used. Will return the object or null
	objectclass *useup();
	// These two are called when an item is picked up and when it is dropped.
	// They are needed for items which do things simply by carrying them.
	virtual void carry(){}
	virtual void drop(){}
};

#endif
