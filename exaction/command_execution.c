/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahaded <aahaded@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 08:22:07 by aahaded           #+#    #+#             */
/*   Updated: 2025/02/17 08:22:09 by aahaded          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static int is_directory(char *cmd)
// {
// 	struct stat statbuf;

// 	if (stat(cmd, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
// 	{
// 		fprintf(stderr, "minishell: %s: Is a directory\n", cmd);
// 		exit(126);
// 	}
// 	return (0);
// }

// static int contains_slash(char *cmd)
// {
// 	int i;

// 	i = 0;
// 	while (cmd[i])
// 	{
// 		if (cmd[i] == '/')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// static void handle_exec_failure(char *cmd, int check_)
// {
// 	if (check_)
// 	{
// 		fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
// 		exit(127);
// 	}
// 	fprintf(stderr, "minishell: %s: Command not found\n", cmd);
// 	exit(127);
// }

// static int handle_exit_status(t_data *data, int status)
// {

// 	if (WEXITSTATUS(status) == 0)
// 	{
// 		data->exit_status = 0;
// 		return (SUCCESS);
// 	}
// 	else if (WEXITSTATUS(status) == 126)
// 	{
// 		data->exit_status = 126;
// 		return (FAILED);
// 	}
// 	else if (WEXITSTATUS(status) == 127)
// 	{
// 		data->exit_status = 127;
// 		return (FAILED);
// 	}
// 	else
// 		data->exit_status = 1;
// 	return (FAILED);
// }

// int execute_command(char **cmd, t_data *data)
// {
// 	pid_t pid;
// 	int status, exit_s;

// 	pid = fork();
// 	if (pid == -1)
// 		return (FAILED);
// 	if (pid == 0)
// 	{
// 		is_directory(cmd[0]);
// 		if (get_path_env(cmd[0], data) != NULL && !contains_slash(cmd[0]))
// 		{
// 			execve(get_path_env(cmd[0], data), cmd, data->env);
// 			dup2(data->stdout_backup, STDOUT_FILENO);
// 			close(data->stdout_backup);
// 		}
// 		else
// 		{
// 			handle_exec_failure(cmd[0], contains_slash(cmd[0]));
// 			dup2(data->stdout_backup, STDOUT_FILENO);
// 			close(data->stdout_backup);
// 		}
// 		return (FAILED);
// 	}
// 	wait(&status);
// 	if (WIFEXITED(status))
// 	{
// 		exit_s = handle_exit_status(data, status);
// 		if (exit_s == FAILED)
// 		{
// 			dup2(data->stdout_backup, STDOUT_FILENO);
// 			close(data->stdout_backup);
// 			return (FAILED);
// 		}
// 		else if (exit_s == SUCCESS)
// 		{
// 			dup2(data->stdout_backup, STDOUT_FILENO);
// 			close(data->stdout_backup);
// 			return (SUCCESS);
// 		}
// 	}
// 	dup2(data->stdout_backup, STDOUT_FILENO);
// 	close(data->stdout_backup);
// 	return (FAILED);
// }

int execute_command(char **cmd, t_data *data)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
        return (FAILED);
    else if (pid == 0)
    {
        execve(get_path_env(cmd[0], data), cmd, data->env);
        ft_printf("Minishell: %s: Command not found\n", cmd[0]);
        exit(FAILED);
    }
    else
    {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            data->exit_status = WEXITSTATUS(status);
            return (SUCCESS);
        }
        else
            data->exit_status = 127;
        return (FAILED);
    }
}