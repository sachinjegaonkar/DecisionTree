CREATE TABLE `spaceshuttle` (
  `rings` varchar(20) default NULL,
  `thermaldistress` varchar(20) default NULL,
  `temperature` varchar(20) default NULL,
  `pressure` varchar(20) default NULL,
  `class` varchar(20) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/* Data for the `spaceshuttle` table  (Records 1 - 23) */

INSERT INTO `spaceshuttle` (`rings`, `thermaldistress`, `temperature`, `pressure`, `class`) VALUES 
  ('six', 'zero', '61...70', 'low', 'acc'),
  ('six', 'one', '61...70', 'low', 'acc'),
  ('six', 'zero', '61...70', 'low', 'acc'),
  ('six', 'zero', '61...70', 'low', 'acc'),
  ('six', 'zero', '61...70', 'low', 'acc'),
  ('six', 'zero', '>70', 'low', 'acc'),
  ('six', 'zero', '>70', 'medium', 'acc'),
  ('six', 'zero', '>70', 'medium', 'acc'),
  ('six', 'one', '<=60', 'high', 'acc'),
  ('six', 'one', '61...70', 'high', 'acc'),
  ('six', 'one', '61...70', 'high', 'acc'),
  ('six', 'zero', '>70', 'high', 'acc'),
  ('six', 'zero', '61...70', 'high', 'acc'),
  ('six', 'two', '<=60', 'high', 'unacc'),
  ('six', 'zero', '61...70', 'high', 'acc'),
  ('six', 'zero', '>70', 'high', 'acc'),
  ('six', 'zero', '61...70', 'high', 'acc'),
  ('six', 'zero', '>70', 'high', 'acc'),
  ('six', 'zero', '>70', 'high', 'acc'),
  ('six', 'zero', '>70', 'high', 'unacc'),
  ('six', 'two', '>70', 'high', 'unacc'),
  ('six', 'zero', '>70', 'high', 'acc'),
  ('six', 'one', '<=60', 'high', 'acc');


