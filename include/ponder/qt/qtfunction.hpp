/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#ifndef PONDER_QT_QTFUNCTION_HPP
#define PONDER_QT_QTFUNCTION_HPP


#include <ponder/qt/qthelper.hpp>
#include <ponder/function.hpp>
#include <ponder/userobject.hpp>
#include <ponder/value.hpp>
#include <QMetaMethod>
#include <QVariant>
#include <QString>


namespace ponder_ext
{
/**
 * \brief Specialization of ponder::Function implemented using a Qt method
 *
 * This class is instanciated and returned by QtMapper<T>.
 *
 * \sa QtMapper
 */
template <typename T>
class QtFunction : public ponder::Function
{
public:

    /**
     * \brief Construct the function from a QMetaMethod
     *
     * \param metaMethod Qt meta method
     */
    QtFunction(const QMetaMethod& metaMethod)
        : ponder::Function(functionName(metaMethod), returnType(metaMethod), argTypes(metaMethod))
        , m_metaMethod(metaMethod)
    {
    }

    /**
     * \see Function::execute
     */
    virtual ponder::Value execute(const ponder::UserObject& object, const ponder::Args& args) const override
    {
        QVariant value(QtHelper::variantType(QMetaType::type(m_metaMethod.typeName())));
        QGenericReturnArgument ret(m_metaMethod.typeName(), value.data());

        switch (args.count())
        {
            case 0:
            {
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret);
                break;
            }

            case 1:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data()));
                break;
            }

            case 2:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data()));
                break;
            }

            case 3:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data()));
                break;
            }

            case 4:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                QVariant arg4 = QtHelper::valueToVariant(args[3]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data())
                                  , QGenericArgument(arg4.typeName(), arg4.data()));
                break;
            }

            case 5:
            {
                QVariant arg1 = QtHelper::valueToVariant(args[0]);
                QVariant arg2 = QtHelper::valueToVariant(args[1]);
                QVariant arg3 = QtHelper::valueToVariant(args[2]);
                QVariant arg4 = QtHelper::valueToVariant(args[3]);
                QVariant arg5 = QtHelper::valueToVariant(args[4]);
                m_metaMethod.invoke(object.get<T*>(), Qt::DirectConnection, ret
                                  , QGenericArgument(arg1.typeName(), arg1.data())
                                  , QGenericArgument(arg2.typeName(), arg2.data())
                                  , QGenericArgument(arg3.typeName(), arg3.data())
                                  , QGenericArgument(arg4.typeName(), arg4.data())
                                  , QGenericArgument(arg5.typeName(), arg5.data()));
                break;
            }
        }

        return QtHelper::variantToValue(value);
    }

private:

    /**
     * \brief Extract the name of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Name of the meta method
     */
    static std::string functionName(const QMetaMethod& metaMethod)
    {
        QString signature = metaMethod.signature();
        return signature.left(signature.indexOf('(')).toStdString();
    }

    /**
     * \brief Extract the return type of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Return type of the meta method
     */
    static ponder::Type returnType(const QMetaMethod& metaMethod)
    {
        return QtHelper::type(metaMethod.typeName());
    }

    /**
     * \brief Extract the arguments types of a meta method
     *
     * \param metaMethod Qt meta method
     *
     * \return Arguments types of the meta method
     */
    static std::vector<ponder::Type> argTypes(const QMetaMethod& metaMethod)
    {
        std::vector<ponder::Type> types;

        QList<QByteArray> args = metaMethod.parameterTypes();
        Q_FOREACH(QByteArray arg, args)
        {
            types.push_back(QtHelper::type(arg));
        }

        return types;
    }

private:

    QMetaMethod m_metaMethod; ///< Internal Qt method
};

} // namespace ponder_ext


#endif // PONDER_QT_QTFUNCTION_HPP
