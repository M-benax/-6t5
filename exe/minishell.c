/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:52:12 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/31 22:40:17 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../minishell.h" // تأكد أن هذا هو المسار الصحيح

// إذا لم يكن لديك هذه، أضفها (أو ما يعادلها)
void gc_free_token_list(t_comand *list) {
    t_comand *tmp;
    while (list) {
        tmp = list;
        list = list->next;
        gc_free_ptr(tmp->word); // افترض أن word مخصص بـ gc_malloc
        gc_free_ptr(tmp);
    }
}

void gc_free_cmd_structure(t_cmd *cmd_list) {
    t_cmd *current_cmd;
    t_cmd *next_cmd;
    int i;

    current_cmd = cmd_list;
    while (current_cmd) {
        next_cmd = current_cmd->next;
        if (current_cmd->args) {
            i = 0;
            while (current_cmd->args[i]) {
                gc_free_ptr(current_cmd->args[i]);
                i++;
            }
            gc_free_ptr(current_cmd->args);
        }
        gc_free_ptr(current_cmd->in_file); // إذا كان دائمًا مخصصًا
        gc_free_ptr(current_cmd->out_file); // إذا كان دائمًا مخصصًا
        gc_free_ptr(current_cmd);
        current_cmd = next_cmd;
    }
}


int main(int ac, char **av, char **env)
{
    char *line;
    t_comand *token_list; // غيرت الاسم ليكون أوضح
    t_data *data;

    (void)ac;
    (void)av;
    data = gc_malloc(sizeof(t_data));
    if (!data) // تحقق من نجاح gc_malloc
        return (1);
    data->tenv = init_env(env);
    data->com = NULL; // تهيئة أولية
    set_global_exit_status(0); // تهيئة $?

    while (1)
    {
        signal_init(); // يجب أن تكون داخل الحلقة لمعالجة Ctrl+C بشكل صحيح لكل أمر جديد
        line = readline("minishell-$ ");
        if (!line) // Ctrl+D
        {
            printf("exit\n"); // سلوك Bash
            gc_free_all(); // حرر كل شيء قبل الخروج
            // rl_clear_history(); // هذه الدالة غير موجودة في قائمة الدوال المسموح بها
            exit(get_global_exit_status()); // اخرج بآخر exit status
        }

        if (line[0] != '\0') // لا تقم بإضافة الأسطر الفارغة إلى الهيستوري
            add_history(line);

        token_list = tokens(line); // افترض أن tokens تعالج السلاسل الفارغة وتعيد NULL

        // تحقق من الـ Syntax أولاً
        if (token_list && check_syntax(token_list) != 0) // افترض أن check_syntax ترجع 0 للنجاح
        {
            // printf("minishell: syntax error\n"); // رسالة خطأ عامة
            set_global_exit_status(2); // Bash غالبًا ما يستخدم 2 لـ syntax errors
            free(line);
            gc_free_token_list(token_list); // حرر قائمة التوكنز
            continue;
        }

        if (token_list) // فقط إذا كان هناك توكنز صالحة
        {
            data->com = parse_command(token_list);
            // gc_free_token_list(token_list); // حرر التوكنز بعد أن استخدمها parse_command

            if (data->com)
            {
                printf("[DEBUG main] Before expand_all_commands\n");
                expand_all_commands(data); // يجب أن تعالج الأخطاء إذا كانت موجودة وتضبط $?
                printf("[DEBUG main] Before start_execution\n");
                // biltings(data); // <<<<<< تم التعليق عليها، start_execution ستعالج الـ builtins
                start_execution(data);
                printf("[DEBUG main] After start_execution, $? = %d\n", get_global_exit_status());
            }
           // gc_free_cmd_structure(data->com); // حرر هيكل الأوامر بعد التنفيذ
           // data->com = NULL; // أعد تعيينه
        }
        free(line); // حرر السطر المقروء
        // الـ gc_free_all() ستكون في حالة الـ exit النهائي
        // أو يمكنك تحرير الـ cmd structure هنا إذا لم تكن متأكدًا من أن gc_free_all تعالجه
    }
    gc_free_all(); // نظريًا لا يصل إلى هنا إلا إذا كسرنا الحلقة بشكل آخر
    // rl_clear_history();
    return (get_global_exit_status());
}










































// typedef struct s_cmd
// {
//     char **args;       // arguments
//     int    in_type;    // < أو <<
//     char  *in_file;    // الملف لي غادي يقرا منو
//     int    out_type;   // > أو >>
//     char  *out_file;   // الملف لي غادي يكتب فيه
//     struct s_cmd *next; // باش نربطها مع command أخرى (بسبب pipes)
//     s_data_env env;
// } t_cmd;
