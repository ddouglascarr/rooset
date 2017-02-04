package org.rooset.httpapi.aop;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;

import org.rooset.httpapi.exceptions.RatkException;
import org.rooset.httpapi.enums.ExceptionCode.*;

@Aspect
@Component
public class HandleServiceErrorsAspect
{

    @Around("@annotation(HandleServiceErrors)")
    public ResponseEntity<Object> handleUnprivilegedExecption(ProceedingJoinPoint joinPoint)
    {
        try {
            return (ResponseEntity<Object>) joinPoint.proceed();
        } catch (RatkException e) {
            final HttpHeaders httpHeaders= new HttpHeaders();
            httpHeaders.setContentType(MediaType.APPLICATION_JSON);
            switch (e.getExceptionCode()) {
                case ITEM_NOT_FOUND_EXCEPTION:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.NOT_FOUND);
                case CONFLICT_EXCEPTION:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.CONFLICT);
                case UNPRIVILEGED_EXCEPTION:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.UNAUTHORIZED);
                case UNAUTHORIZED_EXCEPTION:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.UNAUTHORIZED);
                case INVALID_INPUT_EXCEPTION:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.BAD_REQUEST);
                default:
                    return new ResponseEntity<>(
                        e.serialize().toString(), httpHeaders, HttpStatus.INTERNAL_SERVER_ERROR);
            }
        } catch (Throwable e) {
            e.printStackTrace();
            return new ResponseEntity<>(HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }
}
