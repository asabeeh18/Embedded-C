if both diff 
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

				
node Check
//belongs to near
if indi[CT]==term[adj][0] & arm free
	adjCount++;
	pickup(0) //pickup returns success or failed pickup ..HOW??
		
	
if indi[CT]==term[adj][1] & arm free 
	adjCount++;
	pickup(1)

//belongs to far
if !(indi[CT]==term[CT][0] && indi[adj]==term[CT][0]) 
	farCount++;
	pickup
	
if !(indi[CT]==term[CT][1] && indi[adj]==term[CT][1])
	farCount++;
	pickup;

if(farCount==2)
	nxTerm=belong[arm1]

else if(adjCount==1)
	if(farCount==1)
		//SchDropAtSort
		nxTerm=adj

	else
		nxTerm=adj

else if(adjCount==2)
	//SchDropAtSort
	nxTerm=adj
else if(adjCount==0==farCount)
		//Term is Done

END nodeCHeck

travel(CT,nxTerm);//Sch included in travel func

solveNode //v r inside d term wid one box of its color and an empty arm
adjCount=0;
farCount=0;

//belongs to near
if indi[CT]==term[adj][0]&& box exists at 0) 
	adjCount++;
	
if indi[CT]==term[adj][1]&& box exists at 1 
	adjCount++;

//belongs to far
if !(indi[CT]==term[CT][0] && indi[adj]==term[CT][0] && box exists at 0) 
	farCount++;
	
if !(indi[CT]==term[CT][1] && indi[adj]==term[CT][1] && box exists at 1)
	farCount++;

if(Sort[CT]==indi[CT])
	if(farCount==2)
		if(term[CT][0]==term[CT][1]) //lil chalaki 
			nxTerm=far; //the common terminal
			//SchDropAtSort
	if(farCount==1 && adjCount==1)
		nxTerm=adj
	if(adjCount==2)
		nxTerm=adj
	pickup
	drop
	goto sort
	pickup
	drop
	goto node
	pickup
	drop
else if(Sort[CT]!=indi[CT])
{
	if(farCount==2)
		if(term[CT][0]==term[CT][1]) //lil chalaki 
			nxTerm=far; //the common terminal
			//SchDropAtSort
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
		else sort belongs to adj
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