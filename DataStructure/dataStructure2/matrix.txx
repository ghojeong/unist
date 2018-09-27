// CSE221 Assignment 3
//Student Number: 20131674
//Student Name :   Jeong  Wan Gho

// Constructor, create empty sparse matrix with (row, col)
template <class type>
Matrix<type>::Matrix(int row, int col)
{
	// ToDo
	nRow=row;
	nCol=col;
	nElem=0;

	int max;
	if(nRow>nCol) max=nRow;
	else max=nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];//making headers

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

//connecting each headers
	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	headnode=new MatrixNodeType(true, t);
	headnode->triple.row=nRow;
	headnode->triple.col=nCol;
	headnode->down=sHead[0];
	headnode->right=sHead[0];
	headnode->next=sHead[0];
	sHead[max-1]->next=headnode;//connecting headnode with the first header.
}
	
// Constructor from istream
template <class type>
Matrix<type>::Matrix(std::ifstream& is)
{
	is >> nRow >> nCol >> nElem;
	int p = (nRow < nCol) ? nCol : nRow;
	
	std::cout << "Row : " << nRow << ", Col : " << nCol << ", nElem : " << nElem << std::endl;

	// ToDo

//bellow here same as the first constructor logic
	int numElem=nElem;
	nElem=0;

	int max=p;
	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	headnode=new MatrixNodeType(true, t);
	headnode->triple.row=nRow;
	headnode->triple.col=nCol;
	headnode->down=sHead[0];
	headnode->right=sHead[0];
	headnode->next=sHead[0];
	sHead[max-1]->next=headnode;
//unitill here, it is same as the first constructor


//input elements in the sparse matrix
	int i,j;
	type val;
	for(int k=0; k<numElem; k++){
		is>>i>>j>>val;
		CreateElement(i,j,val);
	}

}

// Copy constructor
template <class type>
Matrix<type>::Matrix(const Matrix& a)
{
	// ToDo

//bellow here, same as the first constructor logic
	nRow=a.nRow;
	nCol=a.nCol;
	nElem=a.nElem;

	int max;
	if(nRow>nCol) max=nRow;
	else max=nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	headnode=new MatrixNodeType(true, t);
	headnode->triple.row=nRow;
	headnode->triple.col=nCol;
	headnode->down=sHead[0];
	headnode->right=sHead[0];
	headnode->next=sHead[0];
	sHead[max-1]->next=headnode;
//until here, same as the first constructor


//copy the elements in matrix a and input the elemenets by CreateElement Function.
	MatrixNodeType* curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			if(row->triple.value!=0)
				CreateElement(row->triple.row, row->triple.col, row->triple.value);
			row=row->right;
		}
		curr= curr->next;
	}


}
	
// Assignment operator
template <class type>
Matrix<type>& 
Matrix<type>::operator=(const Matrix& a)
{
	// ToDo
//below here same as the destructor logic
	if(this==&a) return *this;
	MatrixNodeType* curr = headnode->next;
	MatrixNodeType*temp;

	while(curr != headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
	delete headnode;
//until here same as the destructor logic

//below same as the first constructor logic
	nRow=a.nRow;
	nCol=a.nCol;
	nElem=a.nElem;

	int max;
	if(nRow>nCol) max=nRow;
	else max=nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	headnode=new MatrixNodeType(true, t);
	headnode->triple.row=nRow;
	headnode->triple.col=nCol;
	headnode->down=sHead[0];
	headnode->right=sHead[0];
	headnode->next=sHead[0];
	sHead[max-1]->next=headnode;
//until here same as the first constructor logic

//below part is same as the copy constructor logic
	curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			if(row->triple.value!=0)
				CreateElement(row->triple.row, row->triple.col, row->triple.value);
			row=row->right;
		}
		curr= curr->next;
	}

	
	return *this;
}
	
// Destructor
template <class type>
Matrix<type>::~Matrix()
{
	// ToDo
	if(!headnode) return;
	MatrixNodeType* curr = headnode->next;
	MatrixNodeType*temp;

	while(curr != headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
//delete the each elements in sparse matrix
			temp= row->right;
			delete row;
			row=temp;
		}
//delete the each header in the sparse matrix
		temp = curr->next;
		delete curr;
		curr=temp;
	}
//delete headnode
	delete headnode;
	nElem=0;
	nRow=0;
	nCol=0;
}

// Add Matrix b to *this
template <class type>
Matrix<type>
Matrix<type>::operator+(const Matrix& b)
{
	Matrix<type> m(*this);

	// ToDo

//check the matrix size is suitable
	if(m.nRow==b.nRow && m.nCol==b.nCol){
		MatrixNodeType* currB = b.headnode->next;
		MatrixNodeType* currM = m.headnode->next;
		while(currB != b.headnode && currM!=m.headnode)
		{
			MatrixNodeType* rowB = currB->right;
			MatrixNodeType* rowM = currM->right;
			while(rowB != currB && rowM!=currM)
			{
//sum the elements in the same position
				if(rowM->triple.col == rowB->triple.col){
					m.CreateElement(rowB->triple.row, rowB->triple.col, rowB->triple.value+rowM->triple.value);
					rowB= rowB->right;
					rowM=rowM->right;
				}
//add element of B if A don¡¯t have any corresponding element to sum
				else if(rowM->triple.col > rowB->triple.col){
					m.CreateElement(rowB->triple.row, rowB->triple.col, rowB->triple.value);
					rowB=rowB->right;
				}
				else if(rowM->triple.col < rowB->triple.col){
					rowM=rowM->right;
				}
			}
			currB= currB->next;
			currM=currM->next;
		}
	}

//return the empty matrix with the same size
	else{
		printf("DIMENSION MISMATCH\n");

	MatrixNodeType* curr = m.headnode->next;
	MatrixNodeType*temp;

	while(curr != m.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
	delete m.headnode;

	m.nRow=1;
	m.nCol=1;
	m.nElem=0;

	int max;
	if(m.nRow>m.nCol) max=m.nRow;
	else max=m.nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	m.headnode=new MatrixNodeType(true, t);
	m.headnode->triple.row=m.nRow;
	m.headnode->triple.col=m.nCol;
	m.headnode->down=sHead[0];
	m.headnode->right=sHead[0];
	m.headnode->next=sHead[0];
		sHead[max-1]->next=m.headnode;
	}
	
	Matrix<type> resut(m);
	m=resut;
	return m;
}
	
//same logic with the adding matrix
// Subtract Matrix b to *this
template <class type>
Matrix<type>
Matrix<type>::operator-(const Matrix& b)
{
	Matrix<type> m(*this);

	// ToDo
	if(m.nRow==b.nRow && m.nCol==b.nCol){
		MatrixNodeType* currB = b.headnode->next;
		MatrixNodeType* currM = m.headnode->next;
		while(currB != b.headnode && currM!=m.headnode)
		{
			MatrixNodeType* rowB = currB->right;
			MatrixNodeType* rowM = currM->right;
			while(rowB != currB && rowM!=currM)
			{
				if(rowM->triple.col == rowB->triple.col){
					m.CreateElement(rowB->triple.row, rowB->triple.col, rowM->triple.value-rowB->triple.value);
					rowB= rowB->right;
					rowM=rowM->right;
				}
				else if(rowM->triple.col > rowB->triple.col){
					m.CreateElement(rowB->triple.row, rowB->triple.col, -(rowB->triple.value));
					rowB=rowB->right;
				}
				else if(rowM->triple.col < rowB->triple.col){
					rowM=rowM->right;
				}
			}
			currB= currB->next;
			currM=currM->next;
		}
	}
	else{
		printf("DIMENSION MISMATCH\n");

	MatrixNodeType* curr = m.headnode->next;
	MatrixNodeType*temp;

	while(curr != m.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
	delete m.headnode;

	m.nRow=1;
	m.nCol=1;
	m.nElem=0;

	int max;
	if(m.nRow>m.nCol) max=m.nRow;
	else max=m.nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	m.headnode=new MatrixNodeType(true, t);
	m.headnode->triple.row=m.nRow;
	m.headnode->triple.col=m.nCol;
	m.headnode->down=sHead[0];
	m.headnode->right=sHead[0];
	m.headnode->next=sHead[0];
		sHead[max-1]->next=m.headnode;
	}

	Matrix<type> resut(m);
	m=resut;

	return m;
}
	
// Multiply Matrix b to *this
template <class type>
Matrix<type>
Matrix<type>::operator*(const Matrix& b)
{
	Matrix<type> m(*this);

	// ToDo
	if(m.nCol==b.nRow){
		Matrix<type> temp(m.nRow, b.nCol);
		type value;


		MatrixNodeType* currB = b.headnode->next;
		MatrixNodeType* currM = m.headnode->next;
		while(currM!=m.headnode)
		{
			while(currB != b.headnode)
			{
				value=0;
				MatrixNodeType* colB = currB->down;
				MatrixNodeType* rowM = currM->right;
				while(colB != currB && rowM!=currM)
				{
//multiply the elements and store as an addition in the value variable
					if(rowM->triple.col == colB->triple.row){
						value+=rowM->triple.value*colB->triple.value;
						colB=colB->down;
						rowM=rowM->right;
					}
					else if(rowM->triple.col > colB->triple.row){
						colB=colB->down;
					}
					else if(rowM->triple.col < colB->triple.row){
						rowM=rowM->right;
					}
				}
				if(value!=0){
//store the concluded value variable at the temp matrix
					temp.CreateElement(currM->triple.row,currB->triple.col, value);
				}
				currB= currB->next;
			}
			currM=currM->next;
			currB=b.headnode->next;
		}
//copy the temp patrix into matrix m
		m=temp;		
	}

//return the empty matrix. Same logic as the addition and subtraction
	else{
		printf("DIMENSION MISMATCH\n");

	MatrixNodeType* curr = m.headnode->next;
	MatrixNodeType*temp;

	while(curr != m.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
	delete m.headnode;

	m.nRow=1;
	m.nCol=1;
	m.nElem=0;

	int max;
	if(m.nRow>m.nCol) max=m.nRow;
	else max=m.nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	m.headnode=new MatrixNodeType(true, t);
	m.headnode->triple.row=m.nRow;
	m.headnode->triple.col=m.nCol;
	m.headnode->down=sHead[0];
	m.headnode->right=sHead[0];
	m.headnode->next=sHead[0];
		sHead[max-1]->next=m.headnode;
	}
	Matrix<type> resut(m);
	m=resut;


	return m;
}
	
// Swap i-th row with j-th row
template <class type>
Matrix<type>& 
Matrix<type>::SwapRow(int i, int j)
{
	// ToDo
	Matrix<type> m(*this);
	MatrixNodeType* currI = m.headnode->next;
	MatrixNodeType* currJ = m.headnode->next;

//get the header of the switching rows
	for(int k=0; k<i; k++){
		currI=currI->next;
	}
	for(int k=0; k<j; k++){
		currJ=currJ->next;
	}


//clean the switching rows in *this, before inputting elements of each rows
	MatrixNodeType* curr = headnode->next;
	MatrixNodeType* temp;
	MatrixNodeType* linkTemp;
	for(int k=0; k<i; k++){
		curr=curr->next;
	}
	MatrixNodeType* row = curr->right;
	while(row != curr)
	{
		linkTemp=row->down;
		while(linkTemp->down!=row){
			linkTemp=linkTemp->down;
		}
		linkTemp->down=row->down;

		temp= row->right;
		delete row;
		row=temp;
	}
	curr->right=curr;

	curr = headnode->next;
	for(int k=0; k<j; k++){
		curr=curr->next;
	}
	row = curr->right;
	while(row != curr)
	{
		linkTemp=row->down;
		while(linkTemp->down!=row){
			linkTemp=linkTemp->down;
		}
		linkTemp->down=row->down;

		temp= row->right;
		delete row;
		row=temp;
	}
	curr->right=curr;

	
//input elements of switching rows in the empty rows which is cleaned
	MatrixNodeType* rowI = currI->right;
	MatrixNodeType* rowJ = currJ->right;
	while(rowI != currI )
	{
		CreateElement(j,rowI->triple.col,rowI->triple.value);
		rowI=rowI->right;
	}
	while(rowJ != currJ )
	{
		CreateElement(i,rowJ->triple.col,rowJ->triple.value);
		rowJ=rowJ->right;
	}


	return *this;
}
	
//same logic as the SwapRow, except switching rows into column and right into down
// Swap i-th col with j-th col
template <class type>
Matrix<type>& 
Matrix<type>::SwapCol(int i, int j)
{
	// ToDo
	Matrix<type> m(*this);
	MatrixNodeType* currI = m.headnode->next;
	MatrixNodeType* currJ = m.headnode->next;

	for(int k=0; k<i; k++){
		currI=currI->next;
	}
	for(int k=0; k<j; k++){
		currJ=currJ->next;
	}




	MatrixNodeType* curr = headnode->next;
	MatrixNodeType* temp;
	MatrixNodeType* linkTemp;
	for(int k=0; k<i; k++){
		curr=curr->next;
	}
	MatrixNodeType* col = curr->down;
	while(col != curr)
	{
		linkTemp=col->right;
		while(linkTemp->right!=col){
			linkTemp=linkTemp->right;
		}
		linkTemp->right=col->right;

		temp= col->down;
		delete col;
		col=temp;
	}
	curr->down=curr;

	curr = headnode->next;
	for(int k=0; k<j; k++){
		curr=curr->next;
	}
	col = curr->down;
	while(col != curr)
	{
		linkTemp=col->right;
		while(linkTemp->right!=col){
			linkTemp=linkTemp->right;
		}
		linkTemp->right=col->right;

		temp= col->down;
		delete col;
		col=temp;
	}
	curr->down=curr;

	
	MatrixNodeType* colI = currI->down;
	MatrixNodeType* colJ = currJ->down;
	while(colI != currI )
	{
		CreateElement(colI->triple.row,j,colI->triple.value);
		colI=colI->down;
	}
	while(colJ != currJ )
	{
		CreateElement(colJ->triple.row,i,colJ->triple.value);
		colJ=colJ->down;
	}


	return *this;
}
	
// Create single element in the matrix
template <class type>
Matrix<type>& 
Matrix<type>::CreateElement(int i, int j, type val)
{
	// ToDo
	MatrixNodeType* currHead;
	MatrixNodeType* newElem;
	MatrixNodeType* currMatrix;
	MatrixNodeType* prevMatrix;
	bool isEmpty=true;
	bool notDone=true;

	TripleType t;
	t.row=i;
	t.col=j;
	t.value=val;

	if( i<nRow && j<nCol){
//first make the element, and it will be connected to matrix
		newElem=new MatrixNodeType(false, &t);
		nElem++;		

		currHead = headnode->next;
//check whether the  row or column is empty
		isEmpty=true;
//check whether the element is already connected to matrix or not
		notDone=true;
		while(currHead != headnode)
		{
			prevMatrix= currHead;
			currMatrix = currHead->right;
			if ( currHead->triple.row==i ){
//connecting the rows with the element first
				while(currMatrix != currHead)
				{
					isEmpty=false;
//replacing the element with the already existing element. 
//already existing element will be deleted after the connection in column
					if (currMatrix->triple.col==j && notDone){
						notDone=false;
						newElem->right=currMatrix->right;
						newElem->down=currMatrix->down;
						if(currMatrix->next!=currHead)
							newElem->next=currMatrix->next;
						else
							newElem->next=currHead->next;
						prevMatrix->right=newElem;
						if(prevMatrix!=currHead)
							prevMatrix->next=newElem;
						currMatrix=newElem;
						break;
					}
//connecting the element between two elements
					if (currMatrix->triple.col>j && notDone){
						notDone=false;
						newElem->right=currMatrix;
						newElem->next=currMatrix;
						prevMatrix->right=newElem;
						if(prevMatrix!=currHead)
							prevMatrix->next=newElem;
						prevMatrix=newElem;
						break;
					}
					prevMatrix=currMatrix;
					currMatrix = currMatrix->right;
				}
//connecting the element with the header if there is it is empty
				if(isEmpty && notDone){
					notDone=false;
					newElem->right=currHead;
					newElem->next=currHead->next;
					currHead->right=newElem;
				}
//connecting the element when it is at the end of the row
				else if(notDone){
					notDone=false;
					newElem->right=currHead;
					newElem->next=currHead->next;
					prevMatrix->right=newElem;
					if(prevMatrix!=currHead)
						prevMatrix->next=newElem;
				}
				break;
			}
			if(!notDone){break;}
			currHead = currHead->next;
		}

//connecting the element in the column
		currHead = headnode->next;
		isEmpty=true;
		notDone=true;
		while(currHead != headnode)
		{
			prevMatrix= currHead;
			currMatrix = currHead->down;
			if ( currHead->triple.col==j ){

				while(currMatrix != currHead)
				{
					isEmpty=false;
//after replacing the already existing element, already existing element is deleted
					if (currMatrix->triple.row==i && notDone){
						notDone=false;
						prevMatrix->down=newElem;
						delete currMatrix;
						nElem--;
						currMatrix=newElem;
						break;
					}
//connecting the elelment between two elements in column
					if (currMatrix->triple.col>j && notDone){
						notDone=false;
						newElem->down=currMatrix;
						prevMatrix->down=newElem;
						prevMatrix=newElem;
						break;
					}
					prevMatrix=currMatrix;
					currMatrix = currMatrix->down;
				}
//connecting the element in column when the matrix is empty
				if(isEmpty && notDone){
					notDone=false;
					currHead->down=newElem;
					newElem->down=currHead;
				}
//connecting the element when it is located at the end of column
				else if(notDone){
					notDone=false;
					newElem->down=currHead;
					prevMatrix->down=newElem;
				}
				break;
			}
			if(!notDone){break;}
			currHead = currHead->next;
		}
	}

	return *this;
}
	
// Transpose matrix
template <class type>
Matrix<type>& 
Matrix<type>::Transpose()
{
	// ToDo
//copy this matrix at a
	Matrix a(*this);
	MatrixNodeType* curr = headnode->next;
	MatrixNodeType*temp;

//below is same as the destructor
	while(curr != headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
	delete headnode;
//until here is the same as the destructor

//below here is same as the first constructor
	nRow=a.nRow;
	nCol=a.nCol;
	nElem=a.nElem;

	int max;
	if(nRow>nCol) max=nRow;
	else max=nCol;

	TripleType* t;
	MatrixNodeType* sHead[max];

	for(int i=0; i<max; i++){
		sHead[i]=new MatrixNodeType(true, t);
		sHead[i]->triple.row=i;
		sHead[i]->triple.col=i;
	}

	for(int i=0; i<max-1; i++){
			sHead[i]->next=sHead[i+1];
	}
	headnode=new MatrixNodeType(true, t);
	headnode->triple.row=nRow;
	headnode->triple.col=nCol;
	headnode->down=sHead[0];
	headnode->right=sHead[0];
	headnode->next=sHead[0];
	sHead[max-1]->next=headnode;
//until here is same as the first constructor

//copy the elements in transpose direction
	curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			if(row->triple.value!=0)
				CreateElement(row->triple.col, row->triple.row, row->triple.value);
			row=row->right;
		}
		curr= curr->next;
	}

	return *this;
}
