<?php
/* new multi implement test */
$server = new Swoole\Http\Server("127.0.0.1", 9502, SWOOLE_BASE);

$server->set([
	'worker_num' => 1,
]);

$server->on('Request', function ($request, $response) {
	$redis = new Swoole\Coroutine\Redis();
	$res = $redis->connect('127.0.0.1', 6379);
	if ($res == false) {
		$response->end("Redis connect fail!");
		return;
	}
	$redis->defer(true);
	$redis->get('key');

	var_dump($redis->defer());//get true
	var_dump($redis->defer(false));//get false

	//穿插其他client也能正常工作
	$redis_tmp = new Swoole\Coroutine\Redis();
	$res = $redis_tmp->connect('127.0.0.1', 6379);
	if ($res == false) {
		$response->end("Redis connect fail!");
		return;
	}
	$res = $redis_tmp->set('key_tmp', 'HaHa');//get true
	var_dump($res);

	$mysql = new Swoole\Coroutine\MySQL();
	$res = $mysql->connect(['host' => '192.168.244.128', 'user' => 'mha_manager', 'password' => 'mhapass', 'database' => 'tt']);
	if ($res == false) {
		$response->end("MySQL connect fail!");
		return;
	}
	$mysql->defer(true);
	$mysql->query('select sleep(1)', 2);

	$udp = new Swoole\Coroutine\Client(SWOOLE_SOCK_UDP);
    $res = $udp->connect("127.0.0.1", 9906, 2);
	$udp->send('Hello World!');

	//穿插其他client也能正常工作
	$udp_tmp = new Swoole\Coroutine\Client(SWOOLE_SOCK_UDP);
    $res = $udp_tmp->connect("127.0.0.1", 9909, 2);//nonexistent server
	$res = $udp_tmp->recv();//get false with timeout
	var_dump($res);

	$udp_res = $udp->recv();
	$mysql_res = $mysql->recv();
	$redis_res = $redis->recv();

	var_dump($udp_res, $mysql_res, $redis_res);
	$response->end('Test End');
});
$server->start();
