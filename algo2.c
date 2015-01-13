//MOST IDIOTIC AND REPETITIVE TYPE of code ever written

/*if both diff 
	pick both
	place 1 at swap
	check adj
	if-adj color found do it first
	worst case-far found
		pick d one which is in hand
		schdrop at swap
		go to its term 
		if full
			pick d one whose term has space
			drop box
		pick from swap
		drop other box
		move in 
			pick and drop at other node
		get out
			check if box at swap's term has a free node
				pick
*/
	
Basically
	check the 1st  Term and follow through keeping its boxing at specific terminals
	
	box found ..keep it at its Term...loop and done
	
Features
-Cross the bridge only with 2 hands full  60% probab
-empty nodes
-AT MAX 4 times crossing the bridge 20% probab






node Check
//belongs to near
if indi[CT]==term[adj][0] & arm free
	adjCount++;
	//pickup(0) //pickup returns success or failed pickup ..HOW??
		
	
if indi[CT]==term[adj][1] & arm free 
	adjCount++;
	//pickup(1)

//belongs to far
if !(indi[CT]==term[CT][0] && indi[adj]==term[CT][0]) 
	farCount++;
	//pickup
	
if !(indi[CT]==term[CT][1] && indi[adj]==term[CT][1])
	farCount++;
	//pickup;

pickup all //for 1st term only
if(farCount==2)
	nxTerm=belong[arm1]

else if(adjCount==1)
		if(farCount==1)
			//SchDropAtSort
			nxTerm=adj
		else
			//empty node
			nxTerm=adj;
			
		
else if(farCount==1)
		if(adjCount==1)
			//SchDropAtSort
			nxTerm=adj
		else
			nxTerm=adj
			//SchDropAtSort
			//empty node
			//:O :O :O :O :O 

else if(adjCount==2)
	//SchDropAtSort
	nxTerm=adj
else if(adjCount==0==farCount)
		nxTerm=which is not yet visited or which is not yet done//Term is Done
		//or empty Term

END nodeCHeck

travel(CT,nxTerm);//Sch included in travel func

solveNode //v r inside d term wid one box of its color and an empty arm and MAYBE 1 box at sort
adjCount=0;
farCount=0;

//belongs to near
if indi[CT]==term[adj][0]) 
	adjCount++;
	
if indi[CT]==term[adj][1]
	adjCount++;

//belongs to far
if !(indi[CT]==term[CT][0] && indi[adj]==term[CT][0] ) 
	farCount++;
	
if !(indi[CT]==term[CT][1] && indi[adj]==term[CT][1])
	farCount++;

if(Sort[CT]==indi[CT])
	if(farCount==2)
		if(term[CT][0]==term[CT][1]) //lil chalaki 
			nxTerm=far; //the common terminal
		else
			//both diff of far
			nxTerm=far
		pickup
		drop
		goto sort
		pickup 
		drop
		goto CT
		pickup 
		drop
		//SchPickupAtSort	//CT
		//SchDropAtSort		//far
	
	if(farCount==1)
		if(adjCount==1)
			nxTerm=adj
			pickup	far
			drop
			goto sort
			pickup 
			drop
			goto CT
			pickup 
			drop
			//nothing to schedule
		else
			//empty node
			//:O :O
			if adj is not visited or not yet completed(has a far box)
				nxTerm=adj
				
			else
				nxTerm=far
			pickup
			drop
			goto Sort
			pickup
			goto CT
			drop
			
	if(adjCount==1)
		if(farCount==1)
			nxTerm=adj
		else
			//empty node
			nxTerm=adj
	
	if(adjCount==2)
		nxTerm=adj
		pickup 
		drop
		goto sort
		pickup
		drop
		goto CT
		pickup
		drop
		

else if(Sort[CT]!=indi[CT])
{
	if(farCount==2)
		if(term[CT][0]==term[CT][1] && sort[CT] not empty):
			nxTerm=far; //the common terminal
			//SchPickupAtSort
			//SchDropAtSort
			pickup
			drop
			goto sort
			pickup 
			drop
			goto CT
			pickup drop
			
		else if(term[CT][0]==Sort[CT])
			nxTerm=To wherever it belongs
			pickup 0
			drop
			//SchPickupAtSort
		else if(term[CT][1]==Sort[CT])
			nxTerm=To wherever it belongs
			pickup 1
			drop
			//SchPickupAtSort
		else sort belongs to adj or is empty
				pickup // the box which belongs to term with empty node
				drop
				//SchPickupAtSort
				
		/* at this point adj will atleast hav one far box and atmost 1 adj box 
		and 1 box not belonging to it*/
	if(farCount==1)
		if(adjCount==1)
			if(Sort[CT]==indi[adj])
				pickup adjwala
				drop
				//SchPickupAtSort
			else
				//Sort of far and CT has 1 far a adj
				if(Sort[CT]==
		else
			//one node is empty
			pickup
			drop
			nxTerm=adj //for Sort
	if(adjCount==1) //1 empty 1 adj
		//one node is empty
			pickup
			drop
			nxTerm=adj //for Sort
}

SchPickupAtSort()
{
	schP[CT]=1;
}
SchDropAtSort()
{
	schD[CT]=1;
}


/* logic flaw  ..solved
1.	1		2.		X
  0  0			x		x
  
3.    0  	4.			x
  1   1        x        x
  
  wont go to X term's
  sort 1 and 0 and stops
 */
 
 update:indi stores the term(value) of corres color(index)
 
 /*
 lcd_cursor(1,1);  //set the cursor on row 1, column 1
	lcd_string("Blue Pulses"); // Display "Blue Pulses" on LCD
	lcd_print(2,1,blue,5);  //Print the count on second row
	_delay_ms(1000);	// Display for 1000ms or 1 second
	lcd_wr_command(0x01); //Clear the LCD
*/