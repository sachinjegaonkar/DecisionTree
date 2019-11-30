REATE TABLE `shop` (
  `age` varchar(20) default NULL,
  `income` varchar(20) default NULL,
  `student` varchar(20) default NULL,
  `credit` varchar(20) default NULL,
  `class` varchar(20) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/* Data for the `shop` table  (Records 1 - 14) */

INSERT INTO `shop` (`age`, `income`, `student`, `credit`, `class`) VALUES 
  ('<=30', 'High', 'No', 'Fair', 'No'),
  ('<=30', 'High', 'No', 'Excellent', 'No'),
  ('31...40', 'High', 'No', 'Fair', 'Yes'),
  ('>40', 'Medium', 'No', 'Fair', 'Yes'),
  ('>40', 'Low', 'Yes', 'Fair', 'Yes'),
  ('>40', 'Low', 'Yes', 'Excellent', 'No'),
  ('31...40', 'Low', 'Yes', 'Excellent', 'Yes'),
  ('<=30', 'Medium', 'No', 'Fair', 'No'),
  ('<=30', 'Low', 'Yes', 'Fair', 'Yes'),
  ('>40', 'Medium', 'Yes', 'Fair', 'Yes'),
  ('<=30', 'Medium', 'Yes', 'Excellent', 'Yes'),
  ('31...40', 'Medium', 'No', 'Excellent', 'Yes'),
  ('31...40', 'High', 'Yes', 'Fair', 'Yes'),
  ('>40', 'Medium', 'No', 'Excellent', 'No');


