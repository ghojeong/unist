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
}
	
// Constructor from istream
template <class type>
Matrix<type>::Matrix(std::ifstream& is)
{
	is >> nRow >> nCol >> nElem;
	int p = (nRow < nCol) ? nCol : nRow;
	
	std::cout << "Row : " << nRow << ", Col : " << nCol << ", nElem : " << nElem << std::endl;

	// ToDo

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

	MatrixNodeType* curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
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

	curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
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
			temp= row->right;
			delete row;
			row=temp;
		}
		temp = curr->next;
		delete curr;
		curr=temp;
	}
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
					m.CreateElement(rowB->triple.row, rowB->triple.col, rowB->triple.value+rowM->triple.value);
					rowB= rowB->right;
					rowM=rowM->right;
				}
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

	return m;
}
	
// Subtract Matrix b to *this
template <class type>
Matrix<type>
Matrix<type>::operator-(const Matrix& b)
{
	Matrix<type> m(*this);

	// ToDo

	return m;
}
	
// Multiply Matrix b to *this
template <class type>
Matrix<type>
Matrix<type>::operator*(const Matrix& b)
{
	Matrix<type> m(*this);

	// ToDo

	return m;
}
	
// Swap i-th row with j-th row
template <class type>
Matrix<type>& 
Matrix<type>::SwapRow(int i, int j)
{
	// ToDo

	return *this;
}
	
// Swap i-th col with j-th col
template <class type>
Matrix<type>& 
Matrix<type>::SwapCol(int i, int j)
{
	// ToDo

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
		newElem=new MatrixNodeType(false, &t);
		nElem++;		

		currHead = headnode->next;
		isEmpty=true;

		while(currHead != headnode)
		{
			prevMatrix= currHead;
			currMatrix = currHead->right;
			if ( currHead->triple.row==i ){
				notDone=true;
				while(currMatrix != currHead)
				{
					isEmpty=false;
					if (currMatrix->triple.col==j){
						notDone=false;
						newElem->right=currMatrix->right;
						newElem->down=currMatrix->down;
						if(currMatrix->next!=currHead)
							newElem->next=currMatrix->next;
						else
							newElem->next=currHead->next;
						prevMatrix->right=newElem;
						prevMatrix->next=newElem;
						currMatrix->right=newElem;
						currMatrix->next=newElem;
						currMatrix=newElem;
						break;
					}
					if (currMatrix->triple.col>j){
						notDone=false;
						newElem->right=currMatrix;
						newElem->next=currMatrix;
						prevMatrix->right=newElem;
						prevMatrix->next=newElem;
						prevMatrix=newElem;
						break;
					}
					prevMatrix=currMatrix;
					currMatrix = currMatrix->right;
				}
				if(isEmpty){
					notDone=false;
					newElem->right=currHead;
					newElem->next=currHead->next;
					currHead->right=newElem;
				}
				else if(notDone){
					notDone=false;
					newElem->right=currHead;
					newElem->next=currHead->next;
					prevMatrix->right=newElem;
					prevMatrix->next=newElem;
				}
				break;
			}
			if(!notDone){break;}
			currHead = currHead->next;
		}

		currHead = headnode->next;
		isEmpty=true;
		while(currHead != headnode)
		{
			prevMatrix= currHead;
			currMatrix = currHead->down;
			if ( currHead->triple.col==j ){
				notDone=true;
				while(currMatrix != currHead)
				{
					isEmpty=false;
					if (currMatrix->triple.row==i){
						notDone=false;
						prevMatrix->down=newElem;
						delete currMatrix;
						nElem--;
						currMatrix=newElem;
						break;
					}
					if (currMatrix->triple.col>j){
						notDone=false;
						newElem->down=currMatrix;
						prevMatrix->down=newElem;
						prevMatrix=newElem;
						break;
					}
					prevMatrix=currMatrix;
					currMatrix = currMatrix->down;
				}
				if(isEmpty){
					notDone=false;
					currHead->down=newElem;
					newElem->down=currHead;
				}
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
	else{
		printf("DIMENSION MISMATCH\n");
	}


	return *this;
}
	
// Transpose matrix
template <class type>
Matrix<type>& 
Matrix<type>::Transpose()
{
	// ToDo
	Matrix a(*this);
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

	curr = a.headnode->next;
	while(curr != a.headnode)
	{
		MatrixNodeType* row = curr->right;
		while(row != curr)
		{
			CreateElement(row->triple.col, row->triple.row, row->triple.value);
			row=row->right;
		}
		curr= curr->next;
	}

	return *this;
}




