SimpleOrm
=========

simple data Orm based on PDO

# Installing/Configuring
```
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
```
Then add
```
extension="SimpleOrm.so"
```
to your php.ini

#Usage

###SimpleOrm::getInstance(PDO $pdo)
```php
<?php
$dsn = "mysql:host=localhost;dbname=sakila";
$pdo= new PDO($dsn, 'root', '');
$obj=SimpleOrm::getInstance($pdo);
```
###SimpleOrm::query(string $query)
```php
<?php
$obj->query('select * from actor');
```

###SimpleOrm::exec(string $query)
```php
<?php
//excute a query
$obj->exec('insert into actor (id, name, age)');
```

###SimpleOrm::select(string $table)
```php
<?php
//chose a table
$obj->select('actor');
```

###SimpleOrm::find(mixed)
```php
<?php
//find all
$obj->select('actor')->find();

//find record where id==2
$obj->select('actor')->find(2);

//find record where id in [1,2,4,5,6]
$obj->select('actor')->find([1,2,4,5,6]);
```

###SimpleOrm::field(string $columns)
```php
<?php
//select columns 
$obj->select('actor')->field('actor_id,first_name,last_name');
```

###SimpleOrm::where(string $where)
```php
<?php
//set where condition 
$obj->select('actor')->field('actor_id,first_name,last_name')->where("where last_name='GABLE'");
```

###SimpleOrm::order(string $order)
```php
<?php
//set order 
$obj->select('actor')->where("where last_name='GABLE'")->order('order by first_name desc');
```

###SimpleOrm::limit(int $limit[, $offset])
```php
<?php
//set limit 
$obj->select('actor')->where("where last_name='GABLE'")->limit(10);
//or
$obj->select('actor')->where("where last_name='GABLE'")->limit(10, 10);
```

###SimpleOrm::top(int $top)
```php
<?php
//get top record
$obj->select('actor')->top(10);
```

###SimpleOrm::end(int $end)
```php
<?php
//get last record
$obj->select('actor')->end(10);
```

###SimpleOrm::insert(string $table, array $data)
```php
<?php
//insert one record
$obj->insert('actor', array(...));
//or
$obj->table='actor';
$obj->insert(array(...));
```
