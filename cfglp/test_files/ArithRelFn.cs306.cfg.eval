Evaluating Program
   Global Variables (before evaluating):
   Evaluating Procedure << main >>
      Local Variables (before evaluating):
         a : undefined
         b : undefined
         c : undefined


      Basic Block: 2

         Asgn:
            LHS (Name : a)
            RHS (Num : 3)
         a : 3


         Asgn:
            LHS (Name : b)
            RHS (Num : 2)
         b : 2


         Asgn:
            LHS (Name : c)
            RHS (Num : 2.30)
         c : 2.30

   Evaluating Procedure << fn >>
      Local Variables (before evaluating):
         a : undefined


      Basic Block: 2

         Asgn:
            LHS (Name : a)
            RHS (Num : 3)
         a : 3


         RETURN Name : a


      Local Variables (after evaluating) Function: << fn >>
         a : 3
         return : 3

         Asgn:
            LHS (Name : a)
            RHS (
            Arith: MINUS
               LHS (
            Arith: PLUS
               LHS (Name : a)
               RHS (Name : c))
               RHS (
            Arith: DIV
               LHS (
         FN CALL: fn())
               RHS (Name : b)))
         a : 4


         RETURN <NOTHING>


      Local Variables (after evaluating) Function: << main >>
         a : 4
         b : 2
         c : 2.30
   Global Variables (after evaluating):
