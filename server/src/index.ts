import * as commander from 'commander';

commander
  .option('-p, --port <n>', '', parseInt)
  .parse(process.argv);

console.log('Server run(ning) at port', commander.port);