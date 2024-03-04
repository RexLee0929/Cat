document.getElementById('resetPasswordForm').addEventListener('submit', function(event) {
    event.preventDefault(); // 阻止表单的默认提交行为

    const formData = {
        username: document.querySelector('input[name="username"]').value,
        email: document.querySelector('input[name="email"]').value,
        password: document.querySelector('input[name="password"]').value,
    };

    fetch('/api/reset_password', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(formData)
    })
    .then(response => {
        if (!response.ok) {
            // 当响应状态码不是2xx时，尝试从响应体中读取错误信息
            return response.json().then(data => {
                throw new Error(data.error || '网络响应表示失败');
            });
        }
        return response.json();
    })
    .then(data => {
        if (data.message) {
            alert(data.message); // 显示后端返回的成功消息
            window.location.href = 'login.html'; // 密码重置成功后跳转到登录页面
        } else {
            throw new Error('未知错误');
        }
    })
    .catch((error) => {
        console.error('Error:', error);
        alert(error.message); // 显示具体的错误消息
    });
});
