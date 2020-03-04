function nod(n, m) 
{
    if(m > 0) { 
      var k = n%m;
      return nod(m, k); 
    } 
    else { 
      return abs(n);  
    }
}

nod(15, 25)